// #include "test_runner.h"

#include <stdexcept>
#include <vector>

using namespace std;

template <typename T>
class Deque
{
public:
    Deque() = default;

    bool Empty() const noexcept
    {
        return d_front_.empty() && d_back_.empty();
    }

    size_t Size() const noexcept
    {
        return d_front_.size() + d_back_.size();
    }

    const T &operator[](size_t index) const
    {
        if (index < d_front_.size())
        {
            return d_front_[d_front_.size() - index - 1];
        }
        else
        {
            return d_back_[index - d_front_.size()];
        }
    }

    T &operator[](size_t index)
    {
        if (index < d_front_.size())
        {
            return d_front_[d_front_.size() - index - 1];
        }
        else
        {
            return d_back_[index - d_front_.size()];
        }
    }

    const T &At(size_t index) const
    {
        if (index >= Size())
        {
            throw out_of_range("index is out of range");
        }

        return operator[](index);
    }

    T &At(size_t index)
    {
        if (index >= Size())
        {
            throw out_of_range("index is out of range");
        }

        return operator[](index);
    }

    const T &Front() const
    {
        return operator[](0);
    }

    T &Front()
    {
        return operator[](0);
    }

    const T &Back() const
    {
        return operator[](Size() - 1);
    }

    T &Back()
    {
        return operator[](Size() - 1);
    }

    void PushFront(const T &val)
    {
        d_front_.push_back(val);
    }

    void PushBack(const T &val)
    {
        d_back_.push_back(val);
    }

private:
    vector<T> d_front_;
    vector<T> d_back_;
};

// void TestDeque()
// {
//     Deque<int> d;
//     d.PushBack(3);
//     int &a = d[0];

//     const Deque<int> &cd = d;
//     const int &ca = cd[0];

//     ASSERT_EQUAL(a, ca);

//     d.PushFront(2);
//     d.PushFront(1);
//     d.PushFront(0);
//     d.PushBack(4);
//     d.PushBack(5);

//     ASSERT_EQUAL(d.Front(), 0);
//     ASSERT_EQUAL(d.Back(), 5);

//     for (int i = 0; i <= 5; i++)
//     {
//         ASSERT_EQUAL(d[static_cast<size_t>(i)], i);
//         ASSERT_EQUAL(d.At(static_cast<size_t>(i)), i);
//     }

//     try
//     {
//         d.At(6);
//         throw runtime_error("out_of_range exception expected");
//     }
//     catch (const std::out_of_range &)
//     {
//         // OK
//     }
// }

// int main()
// {
//     TestRunner tr;
//     RUN_TEST(tr, TestDeque);
//     return 0;
// }
