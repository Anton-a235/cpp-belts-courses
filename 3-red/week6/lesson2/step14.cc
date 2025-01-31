#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <set>
#include <utility>
#include <vector>

#pragma warning(disable : 4365)

using namespace std;

template <typename T>
class PriorityCollection
{
public:
    using Id = size_t;

    // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object)
    {
        priority_to_ids_[0].insert(next_id_);
        id_to_object_[next_id_] = {move(object), 0};
        return next_id_++;
    }

    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin)
    {
        for (; range_begin != range_end; ++range_begin)
        {
            *ids_begin++ = Add(move(*range_begin));
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const
    {
        return id_to_object_.count(id) > 0;
    }

    // Получить объект по идентификатору
    const T &Get(Id id) const
    {
        auto &[obj, priority] = id_to_object_.at(id);
        return obj;
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id)
    {
        auto &priority = id_to_object_[id].second;
        priority_to_ids_[priority].erase(id);

        if (priority_to_ids_[priority].size() == 0)
        {
            priority_to_ids_.erase(priority);
        }

        ++priority;
        priority_to_ids_[priority].insert(id);
    }

    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T &, int> GetMax() const
    {
        const Id id = *priority_to_ids_.crbegin()->second.crbegin();
        const auto &[obj, priority] = id_to_object_.at(id);
        return {obj, priority};
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax()
    {
        const Id id = *priority_to_ids_.crbegin()->second.crbegin();
        auto &[obj, priority] = id_to_object_.at(id);
        pair<T, int> result = {move(obj), priority};

        priority_to_ids_[priority].erase(id);
        id_to_object_.erase(id);

        if (priority_to_ids_[priority].size() == 0)
        {
            priority_to_ids_.erase(priority);
        }

        return result;
    }

private:
    inline static int next_id_ = 0;

    map<int, set<Id>> priority_to_ids_;
    map<Id, pair<T, int>> id_to_object_;
};

class StringNonCopyable : public string
{
public:
    using string::string; // Позволяет использовать конструкторы строки
    StringNonCopyable(const StringNonCopyable &) = delete;
    StringNonCopyable(StringNonCopyable &&) = default;
    StringNonCopyable &operator=(const StringNonCopyable &) = delete;
    StringNonCopyable &operator=(StringNonCopyable &&) = default;
};

void TestNoCopy()
{
    PriorityCollection<StringNonCopyable> strings;
    const auto white_id = strings.Add("white");

    vector<const char *> yellow_red{"yellow", "red"};
    vector<PriorityCollection<StringNonCopyable>::Id> yellow_red_ids(2);
    strings.Add(yellow_red.begin(), yellow_red.end(), yellow_red_ids.begin());

    const auto yellow_id = yellow_red_ids[0];
    const auto red_id = yellow_red_ids[1];

    ASSERT(strings.IsValid(white_id));
    ASSERT(strings.IsValid(yellow_id));
    ASSERT(strings.IsValid(red_id));

    ASSERT_EQUAL(strings.Get(white_id), "white");

    strings.Promote(yellow_id);
    for (int i = 0; i < 2; ++i)
    {
        strings.Promote(red_id);
    }
    strings.Promote(yellow_id);
    ASSERT_EQUAL(strings.GetMax().first, "red");
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "red");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "yellow");
        ASSERT_EQUAL(item.second, 2);
    }
    {
        const auto item = strings.PopMax();
        ASSERT_EQUAL(item.first, "white");
        ASSERT_EQUAL(item.second, 0);
    }
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestNoCopy);
    return 0;
}
