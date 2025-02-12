#include "test_runner.h"

#include <iostream>
#include <list>
#include <map>
#include <string>
#include <string_view>
#include <unordered_map>

using namespace std;

struct Record
{
    string id;
    string title;
    string user;
    int timestamp;
    int karma;
};

class Database
{
public:
    bool Put(const Record &record)
    {
        if (id_index_.count(record.id) > 0)
            return false;

        const auto record_it = records_.insert(records_.end(), {record});
        id_index_[record_it->record.id] = record_it;

        auto &timestamp_index_ref = timestamp_index_[record_it->record.timestamp];
        record_it->timestamp_index = timestamp_index_ref.insert(timestamp_index_ref.end(), record_it);

        auto &karma_index_ref = karma_index_[record_it->record.karma];
        record_it->karma_index = karma_index_ref.insert(karma_index_ref.end(), record_it);

        auto &user_index_ref = user_index_[record_it->record.user];
        record_it->user_index = user_index_ref.insert(user_index_ref.end(), record_it);

        return true;
    }

    const Record *GetById(const string &id) const
    {
        const auto it = id_index_.find(id);

        if (it == id_index_.cend())
            return nullptr;

        return &it->second->record;
    }

    bool Erase(const string &id)
    {
        const auto id_it = id_index_.find(id);

        if (id_it == id_index_.cend())
            return false;

        timestamp_index_[id_it->second->record.timestamp].erase(id_it->second->timestamp_index);
        karma_index_[id_it->second->record.karma].erase(id_it->second->karma_index);
        user_index_[id_it->second->record.user].erase(id_it->second->user_index);
        records_.erase(id_it->second);
        id_index_.erase(id_it);

        return true;
    }

    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const
    {
        for (auto it = timestamp_index_.lower_bound(low);
             it != timestamp_index_.upper_bound(high);
             ++it)
        {
            for (const auto &record_it : it->second)
            {
                if (!callback(record_it->record))
                {
                    return;
                }
            }
        }
    }

    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const
    {
        for (auto it = karma_index_.lower_bound(low);
             it != karma_index_.upper_bound(high);
             ++it)
        {
            for (const auto &record_it : it->second)
            {
                if (!callback(record_it->record))
                {
                    return;
                }
            }
        }
    }

    template <typename Callback>
    void AllByUser(const string &user, Callback callback) const
    {
        const auto it = user_index_.find(user);

        if (it == user_index_.cend())
            return;

        for (const auto &record_it : it->second)
        {
            if (!callback(record_it->record))
            {
                return;
            }
        }
    }

private:
    struct IndexedRecord;
    list<IndexedRecord> records_;

    using RecordIterator = decltype(records_)::iterator;
    using RecordIteratorList = list<RecordIterator>;
    unordered_map<string_view, RecordIterator> id_index_;
    map<int, RecordIteratorList> timestamp_index_;
    map<int, RecordIteratorList> karma_index_;
    unordered_map<string_view, RecordIteratorList> user_index_;

    struct IndexedRecord
    {
        Record record;
        decltype(timestamp_index_)::mapped_type::iterator timestamp_index;
        decltype(karma_index_)::mapped_type::iterator karma_index;
        decltype(user_index_)::mapped_type::iterator user_index;
    };
};

void TestRangeBoundaries()
{
    const int good_karma = 1000;
    const int bad_karma = -10;

    Database db;
    db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
    db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

    int count = 0;
    db.RangeByKarma(bad_karma, good_karma,
                    [&count](const Record &)
                    {
                        ++count;
                        return true;
                    });

    ASSERT_EQUAL(2, count);

    count = 0;
    db.RangeByTimestamp(1536107260, 1536107260,
                        [&count](const Record &)
                        {
                            ++count;
                            return true;
                        });

    ASSERT_EQUAL(2, count);
}

void TestSameUser()
{
    Database db;
    db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
    db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

    int count = 0;
    db.AllByUser("master",
                 [&count](const Record &)
                 {
                     ++count;
                     return true;
                 });

    ASSERT_EQUAL(2, count);
}

void TestReplacement()
{
    const string final_body = "Feeling sad";

    Database db;
    db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
    db.Erase("id");
    db.Put({"id", final_body, "not-master", 1536107260, -10});

    auto record = db.GetById("id");
    ASSERT(record != nullptr);
    ASSERT_EQUAL(final_body, record->title);
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestRangeBoundaries);
    RUN_TEST(tr, TestSameUser);
    RUN_TEST(tr, TestReplacement);
    return 0;
}
