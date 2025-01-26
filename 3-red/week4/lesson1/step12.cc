#include "step12.h"
#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <utility>

using namespace std;

struct Int
{
    int val;

    Int() : val(0)
    {
        cout << "default Int(" << val << ")" << endl;
    }

    explicit Int(int val) : val(val)
    {
        cout << "explicit Int(" << val << ")" << endl;
    }

    Int(const Int &a) : val(a.val)
    {
        cout << "copy Int(" << val << ")" << endl;
    }

    Int(Int &&a)
    {
        swap(val, a.val);
        cout << "move Int(" << val << ")" << endl;
    }

    Int &operator=(const Int &a)
    {
        if (this == &a)
        {
            return *this;
        }

        val = a.val;
        cout << "copy assignment Int(" << val << ")" << endl;
        return *this;
    }

    Int &operator=(Int &&a)
    {
        swap(val, a.val);
        cout << "move assignment Int(" << val << ")" << endl;
        return *this;
    }

    ~Int()
    {
        cout << "~Int(" << val << ")" << endl;
    }
};

void TestConstruction()
{
    SimpleVector<int> empty;
    ASSERT_EQUAL(empty.Size(), 0u);
    ASSERT_EQUAL(empty.Capacity(), 0u);
    ASSERT(empty.begin() == empty.end());

    SimpleVector<string> five_strings(5);
    ASSERT_EQUAL(five_strings.Size(), 5u);
    ASSERT(five_strings.Size() <= five_strings.Capacity());
    for (auto &item : five_strings)
    {
        ASSERT(item.empty());
    }
    five_strings[2] = "Hello";
    ASSERT_EQUAL(five_strings[2], "Hello");
}

void TestPushBack()
{
    {
        SimpleVector<int> v;
        for (int i = 10; i >= 1; --i)
        {
            v.PushBack(i);
            ASSERT(v.Size() <= v.Capacity());
        }
        sort(begin(v), end(v));

        const vector<int> expected = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        ASSERT_EQUAL(v.Size(), expected.size());
        ASSERT(equal(begin(v), end(v), begin(expected)));
    }
    {
        SimpleVector<Int> v(2);
        for (int i = 1; i < 5; i++)
        {
            cout << "Push " << i << endl;
            v.PushBack(Int(i));
        }

        ASSERT_EQUAL(v.Size(), 6u);
    }
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestConstruction);
    RUN_TEST(tr, TestPushBack);
    return 0;
}
