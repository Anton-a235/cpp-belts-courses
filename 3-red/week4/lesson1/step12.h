// #pragma once

#include <cstdlib>

template <typename T>
class SimpleVector
{
public:
    SimpleVector()
        : data_(nullptr), size_(0), capacity_(0)
    {
    }

    explicit SimpleVector(size_t size)
        : data_(new T[size]), size_(size), capacity_(size)
    {
    }

    ~SimpleVector()
    {
        delete[] data_;
    }

    T &operator[](size_t index)
    {
        return data_[index];
    }

    T *begin()
    {
        return data_;
    }

    T *end()
    {
        return data_ + size_;
    }

    size_t Size() const
    {
        return size_;
    }

    size_t Capacity() const
    {
        return capacity_;
    }

    void PushBack(const T &value)
    {
        if (size_ == capacity_)
        {
            ExpandCapacity();
        }

        data_[size_++] = value;
    }

private:
    T *data_;
    size_t size_;
    size_t capacity_;

    void ExpandCapacity()
    {
        capacity_ = (capacity_ == 0) ? 1 : capacity_ * 2;
        T *new_data = new T[capacity_];

        for (size_t i = 0; i < size_; i++)
        {
            new_data[i] = data_[i]; // std::move(data_[i])
        }

        delete[] data_;
        data_ = new_data;
    }
};
