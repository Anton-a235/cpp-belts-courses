#include "test_runner.h"

#include <cstddef> // нужно для nullptr_t

using namespace std;

template <typename T>
class UniquePtr
{
private:
    T *raw_;

public:
    UniquePtr() noexcept
        : raw_(nullptr)
    {
    }

    UniquePtr(T *ptr) noexcept
        : raw_(ptr)
    {
    }

    UniquePtr(const UniquePtr &) = delete;
    UniquePtr &operator=(const UniquePtr &) = delete;

    UniquePtr(UniquePtr &&other) noexcept
        : UniquePtr()
    {
        Swap(other);
    }

    UniquePtr &operator=(UniquePtr &&other) noexcept
    {
        if (this != &other)
        {
            *this = nullptr;
            Swap(other);
        }

        return *this;
    }

    UniquePtr &operator=(nullptr_t) noexcept
    {
        if (raw_)
        {
            delete raw_;
            raw_ = nullptr;
        }

        return *this;
    }

    ~UniquePtr()
    {
        *this = nullptr;
    }

    T &operator*() const
    {
        return *raw_;
    }

    T *operator->() const noexcept
    {
        return raw_;
    }

    T *Release() noexcept
    {
        T *ret = raw_;
        raw_ = nullptr;
        return ret;
    }

    void Reset(T *ptr) noexcept
    {
        *this = nullptr;
        raw_ = ptr;
    }

    void Swap(UniquePtr &other) noexcept
    {
        swap(raw_, other.raw_);
    }

    T *Get() const noexcept
    {
        return raw_;
    }
};

struct Item
{
    static int counter;
    int value;
    Item(int v = 0)
        : value(v)
    {
        ++counter;
    }
    Item(const Item &other)
        : value(other.value)
    {
        ++counter;
    }
    ~Item()
    {
        --counter;
    }
};

int Item::counter = 0;

void TestLifetime()
{
    Item::counter = 0;
    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        ptr.Reset(new Item);
        ASSERT_EQUAL(Item::counter, 1);
    }
    ASSERT_EQUAL(Item::counter, 0);

    {
        UniquePtr<Item> ptr(new Item);
        ASSERT_EQUAL(Item::counter, 1);

        auto rawPtr = ptr.Release();
        ASSERT_EQUAL(Item::counter, 1);

        delete rawPtr;
        ASSERT_EQUAL(Item::counter, 0);
    }
    ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters()
{
    UniquePtr<Item> ptr(new Item(42));
    ASSERT_EQUAL(ptr.Get()->value, 42);
    ASSERT_EQUAL((*ptr).value, 42);
    ASSERT_EQUAL(ptr->value, 42);
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestLifetime);
    RUN_TEST(tr, TestGetters);
}
