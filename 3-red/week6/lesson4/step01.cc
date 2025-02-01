#pragma warning(disable : 4710)
#pragma warning(disable : 4711)

#include "test_runner.h"
#include "profile.h"

#include <algorithm>
#include <future>
#include <map>
#include <numeric>
#include <vector>
#include <string>
#include <random>

#pragma warning(disable : 4146)
#pragma warning(disable : 4623)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)
#pragma warning(disable : 5026)
#pragma warning(disable : 5027)

using namespace std;

template <typename K, typename V>
class ConcurrentMap
{
public:
    static_assert(is_integral_v<K>, "ConcurrentMap supports only integer keys");

    struct Access
    {
    private:
        lock_guard<mutex> lock_;

    public:
        V &ref_to_value;

        Access(mutex &m, map<K, V> &data, const K &key)
            : lock_(lock_guard(m)), ref_to_value(data[key])
        {
        }
    };

    explicit ConcurrentMap(size_t bucket_count)
        : m_(bucket_count), data_(bucket_count)
    {
    }

    Access operator[](const K &key)
    {
        const size_t index = (key < 0 ? -key : key) % m_.size();
        return {m_[index], data_[index], key};
    }

    map<K, V> BuildOrdinaryMap()
    {
        map<K, V> result;

        for (size_t i = 0; i < m_.size(); ++i)
        {
            lock_guard lock(m_[i]);
            result.insert(data_[i].cbegin(), data_[i].cend());
        }

        return result;
    }

private:
    vector<mutex> m_;
    vector<map<K, V>> data_;
};

void RunConcurrentUpdates(
    ConcurrentMap<int, int> &cm, size_t thread_count, int key_count)
{
    auto kernel = [&cm, key_count](unsigned seed)
    {
        vector<int> updates(static_cast<size_t>(key_count));
        iota(begin(updates), end(updates), -key_count / 2);
        shuffle(begin(updates), end(updates), default_random_engine(seed));

        for (int i = 0; i < 2; ++i)
        {
            for (auto key : updates)
            {
                cm[key].ref_to_value++;
            }
        }
    };

    vector<future<void>> futures;
    for (unsigned i = 0; i < thread_count; ++i)
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

    const auto result = cm.BuildOrdinaryMap();
    ASSERT_EQUAL(result.size(), key_count);
    for (auto &[k, v] : result)
    {
        AssertEqual(v, 6, "Key = " + to_string(k));
    }
}

void TestReadAndWrite()
{
    LOG_DURATION("TestReadAndWrite");
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
        ASSERT(all_of(result.begin(), result.end(), [](const string &s)
                      { return s.empty() || s == "a" || s == "aa"; }));
    }
}

void TestSpeedup()
{
    {
        ConcurrentMap<int, int> single_lock(1);

        LOG_DURATION("TestSpeedup - Single lock");
        RunConcurrentUpdates(single_lock, 4, 50000);
    }
    {
        ConcurrentMap<int, int> many_locks(100);

        LOG_DURATION("TestSpeedup - 100 locks");
        RunConcurrentUpdates(many_locks, 4, 50000);
    }
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestConcurrentUpdate);
    RUN_TEST(tr, TestReadAndWrite);
    RUN_TEST(tr, TestSpeedup);
}
