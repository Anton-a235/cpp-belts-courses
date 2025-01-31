#include "test_runner.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <list>
#include <numeric>
#include <utility>
#include <vector>

#pragma warning(disable : 4365)

using namespace std;

template <typename C>
inline void Advance(typename C::iterator &it, C &c, size_t cur_pos, size_t new_pos)
{
    const auto off = static_cast<ptrdiff_t>(new_pos) - static_cast<ptrdiff_t>(cur_pos);

    if (new_pos < cur_pos && static_cast<ptrdiff_t>(new_pos) < abs(off))
    {
        it = next(c.begin(), new_pos);
    }
    else if (const auto off_from_end = static_cast<ptrdiff_t>(c.size() - new_pos);
             new_pos > cur_pos && off_from_end < off)
    {
        it = prev(c.end(), off_from_end);
    }
    else
    {
        advance(it, off);
    }
}

template <typename RandomIt>
void MakeJosephusPermutation(RandomIt first, RandomIt last, uint32_t step_size)
{
    if (step_size < 2)
    {
        return;
    }

    list<typename RandomIt::value_type> pool(make_move_iterator(first), make_move_iterator(last));
    auto cur_it = pool.begin();
    size_t cur_pos = 0;

    while (!pool.empty())
    {
        *(first++) = move(*cur_it);
        cur_it = pool.erase(cur_it);

        if (pool.empty())
        {
            break;
        }

        size_t new_pos = (cur_pos + step_size - 1) % pool.size();
        Advance(cur_it, pool, cur_pos, new_pos);
        cur_pos = new_pos;
    }
}

vector<int> MakeTestVector()
{
    vector<int> numbers(10);
    iota(begin(numbers), end(numbers), 0);
    return numbers;
}

void TestIntVector()
{
    const vector<int> numbers = MakeTestVector();
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 1);
        ASSERT_EQUAL(numbers_copy, numbers);
    }
    {
        vector<int> numbers_copy = numbers;
        MakeJosephusPermutation(begin(numbers_copy), end(numbers_copy), 3);
        ASSERT_EQUAL(numbers_copy, vector<int>({0, 3, 6, 9, 4, 8, 5, 2, 7, 1}));
    }
}

// Это специальный тип, который поможет вам убедиться, что ваша реализация
// функции MakeJosephusPermutation не выполняет копирование объектов.
// Сейчас вы, возможно, не понимаете как он устроен, однако мы расскажем,
// почему он устроен именно так, далее в блоке про move-семантику —
// в видео «Некопируемые типы»

struct NoncopyableInt
{
    int value;

    NoncopyableInt(const NoncopyableInt &) = delete;
    NoncopyableInt &operator=(const NoncopyableInt &) = delete;

    NoncopyableInt(NoncopyableInt &&) = default;
    NoncopyableInt &operator=(NoncopyableInt &&) = default;
};

bool operator==(const NoncopyableInt &lhs, const NoncopyableInt &rhs)
{
    return lhs.value == rhs.value;
}

ostream &operator<<(ostream &os, const NoncopyableInt &v)
{
    return os << v.value;
}

void TestAvoidsCopying()
{
    vector<NoncopyableInt> numbers;
    numbers.push_back({1});
    numbers.push_back({2});
    numbers.push_back({3});
    numbers.push_back({4});
    numbers.push_back({5});

    MakeJosephusPermutation(begin(numbers), end(numbers), 2);

    vector<NoncopyableInt> expected;
    expected.push_back({1});
    expected.push_back({3});
    expected.push_back({5});
    expected.push_back({4});
    expected.push_back({2});

    ASSERT_EQUAL(numbers, expected);
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestIntVector);
    RUN_TEST(tr, TestAvoidsCopying);
    return 0;
}
