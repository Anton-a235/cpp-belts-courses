#pragma warning(disable : 4710)
#pragma warning(disable : 4711)

#include "profile.h"
#include "test_runner.h"

#include <algorithm>
#include <future>
#include <numeric>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#pragma warning(disable : 4146)
#pragma warning(disable : 4244)
#pragma warning(disable : 4365)
#pragma warning(disable : 4388)
#pragma warning(disable : 4623)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)
#pragma warning(disable : 5026)
#pragma warning(disable : 5027)

using namespace std;

template <typename K, typename V, typename Hash = hash<K>>
class ConcurrentMap
{
public:
    using MapType = unordered_map<K, V, Hash>;

    struct WriteAccess
    {
    private:
        lock_guard<mutex> lock_;

    public:
        V &ref_to_value;

        WriteAccess(mutex &m, MapType &data, const K &key) : lock_(lock_guard(m)), ref_to_value(data[key])
        {
        }
    };

    struct ReadAccess
    {
    private:
        lock_guard<mutex> lock_;

    public:
        const V &ref_to_value;

        ReadAccess(mutex &m, const MapType &data, const K &key) : lock_(lock_guard(m)), ref_to_value(data.at(key))
        {
        }
    };

    explicit ConcurrentMap(size_t bucket_count) : m_(bucket_count), data_(bucket_count), hasher_{}
    {
    }

    WriteAccess operator[](const K &key)
    {
        const size_t index = hasher_(key) % m_.size();
        return {m_[index], data_[index], key};
    }

    ReadAccess At(const K &key) const
    {
        const size_t index = hasher_(key) % m_.size();
        return {m_[index], data_.at(index), key};
    }

    bool Has(const K &key) const
    {
        const size_t index = hasher_(key) % m_.size();
        return data_.at(index).count(key) > 0;
    }

    MapType BuildOrdinaryMap() const
    {
        MapType result;

        for (size_t i = 0; i < m_.size(); ++i)
        {
            lock_guard lock(m_[i]);
            result.insert(data_[i].cbegin(), data_[i].cend());
        }

        return result;
    }

private:
    mutable vector<mutex> m_;
    vector<MapType> data_;
    Hash hasher_;
};

void RunConcurrentUpdates(ConcurrentMap<int, int> &cm, size_t thread_count, int key_count)
{
    auto kernel = [&cm, key_count](int seed)
    {
        vector<int> updates(static_cast<size_t>(key_count));
        iota(begin(updates), end(updates), -key_count / 2);
        shuffle(begin(updates), end(updates), default_random_engine(static_cast<unsigned>(seed)));

        for (int i = 0; i < 2; ++i)
        {
            for (auto key : updates)
            {
                cm[key].ref_to_value++;
            }
        }
    };

    vector<future<void>> futures;
    for (size_t i = 0; i < thread_count; ++i)
    {
        futures.push_back(async(kernel, i));
    }
}

void TestConcurrentUpdate()
{
    const size_t thread_count = 3;
    const size_t key_count = 50000;

    ConcurrentMap<int, int> cm(thread_count);
    RunConcurrentUpdates(cm, thread_count, key_count);

    const auto result = as_const(cm).BuildOrdinaryMap();
    ASSERT_EQUAL(result.size(), key_count);
    for (auto &[k, v] : result)
    {
        AssertEqual(v, 6, "Key = " + to_string(k));
    }
}

void TestReadAndWrite()
{
    ConcurrentMap<size_t, string> cm(5);

    auto updater = [&cm]
    {
        for (size_t i = 0; i < 50000; ++i)
        {
            cm[i].ref_to_value += 'a';
        }
    };
    auto reader = [&cm]
    {
        vector<string> result(50000);
        for (size_t i = 0; i < result.size(); ++i)
        {
            result[i] = cm[i].ref_to_value;
        }
        return result;
    };

    auto u1 = async(updater);
    auto r1 = async(reader);
    auto u2 = async(updater);
    auto r2 = async(reader);

    u1.get();
    u2.get();

    for (auto f : {&r1, &r2})
    {
        auto result = f->get();
        ASSERT(all_of(result.begin(), result.end(),
                      [](const string &s)
                      {
                          return s.empty() || s == "a" || s == "aa";
                      }));
    }
}

void TestSpeedup()
{
    {
        ConcurrentMap<int, int> single_lock(1);

        LOG_DURATION("Single lock");
        RunConcurrentUpdates(single_lock, 4, 50000);
    }
    {
        ConcurrentMap<int, int> many_locks(100);

        LOG_DURATION("100 locks");
        RunConcurrentUpdates(many_locks, 4, 50000);
    }
}

void TestConstAccess()
{
    const unordered_map<int, string> expected = {{1, "one"},
                                                 {2, "two"},
                                                 {3, "three"},
                                                 {31, "thirty one"},
                                                 {127, "one hundred and twenty seven"},
                                                 {1598, "fifteen hundred and ninety eight"}};

    const ConcurrentMap<int, string> cm = [&expected]
    {
        ConcurrentMap<int, string> result(3);
        for (const auto &[k, v] : expected)
        {
            result[k].ref_to_value = v;
        }
        return result;
    }();

    vector<future<string>> futures;
    for (int i = 0; i < 10; ++i)
    {
        futures.push_back(async(
            [&cm, i]
            {
                try
                {
                    return cm.At(i).ref_to_value;
                }
                catch (exception &)
                {
                    return string();
                }
            }));
    }
    futures.clear();

    ASSERT_EQUAL(cm.BuildOrdinaryMap(), expected);
}

void TestStringKeys()
{
    const unordered_map<string, string> expected = {
        {"one", "ONE"},
        {"two", "TWO"},
        {"three", "THREE"},
        {"thirty one", "THIRTY ONE"},
    };

    const ConcurrentMap<string, string> cm = [&expected]
    {
        ConcurrentMap<string, string> result(2);
        for (const auto &[k, v] : expected)
        {
            result[k].ref_to_value = v;
        }
        return result;
    }();

    ASSERT_EQUAL(cm.BuildOrdinaryMap(), expected);
}

struct Point
{
    int x, y;
};

struct PointHash
{
    size_t operator()(Point p) const
    {
        hash<int> h;
        return h(p.x) * 3571 + h(p.y);
    }
};

bool operator==(Point lhs, Point rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

void TestUserType()
{
    ConcurrentMap<Point, size_t, PointHash> point_weight(5);

    vector<future<void>> futures;
    for (int i = 0; i < 1000; ++i)
    {
        futures.push_back(async(
            [&point_weight, i]
            {
                point_weight[Point{i, i}].ref_to_value = i;
            }));
    }

    futures.clear();

    for (int i = 0; i < 1000; ++i)
    {
        ASSERT_EQUAL(point_weight.At(Point{i, i}).ref_to_value, i);
    }

    const auto weights = point_weight.BuildOrdinaryMap();
    for (int i = 0; i < 1000; ++i)
    {
        ASSERT_EQUAL(weights.at(Point{i, i}), i);
    }
}

void TestHas()
{
    ConcurrentMap<int, int> cm(2);
    cm[1].ref_to_value = 100;
    cm[2].ref_to_value = 200;

    const auto &const_map = as_const(cm);
    ASSERT(const_map.Has(1));
    ASSERT(const_map.Has(2));
    ASSERT(!const_map.Has(3));
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestConcurrentUpdate);
    RUN_TEST(tr, TestReadAndWrite);
    RUN_TEST(tr, TestSpeedup);
    RUN_TEST(tr, TestConstAccess);
    RUN_TEST(tr, TestStringKeys);
    RUN_TEST(tr, TestUserType);
    RUN_TEST(tr, TestHas);
}
