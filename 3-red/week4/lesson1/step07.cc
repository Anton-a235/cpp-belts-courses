#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>

using namespace std;

template <class T>
class ObjectPool
{
public:
    T *Allocate()
    {
        T *object = nullptr;

        if (freed_.empty())
        {
            object = new T;
        }
        else
        {
            object = freed_.front();
            freed_.pop();
        }

        allocated_.insert(object);
        return object;
    }

    T *TryAllocate()
    {
        if (freed_.empty())
        {
            return nullptr;
        }

        T *object = freed_.front();
        freed_.pop();
        allocated_.insert(object);

        return object;
    }

    void Deallocate(T *object)
    {
        if (allocated_.erase(object) == 0)
        {
            throw invalid_argument("invalid pointer");
        }

        freed_.push(object);
    }

    ~ObjectPool()
    {
        for (auto &object : allocated_)
        {
            delete object;
        }

        allocated_.clear();

        while (!freed_.empty())
        {
            delete freed_.front();
            freed_.pop();
        }
    }

private:
    set<T *> allocated_;
    queue<T *> freed_;
};

void TestObjectPool()
{
    ObjectPool<string> pool;

    auto p1 = pool.Allocate();
    auto p2 = pool.Allocate();
    auto p3 = pool.Allocate();

    *p1 = "first";
    *p2 = "second";
    *p3 = "third";

    pool.Deallocate(p2);
    ASSERT_EQUAL(*pool.Allocate(), "second");

    pool.Deallocate(p3);
    pool.Deallocate(p1);
    ASSERT_EQUAL(*pool.Allocate(), "third");
    ASSERT_EQUAL(*pool.Allocate(), "first");

    pool.Deallocate(p1);
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestObjectPool);
    return 0;
}
