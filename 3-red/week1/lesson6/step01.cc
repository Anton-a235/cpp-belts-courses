#include "test_runner.h"

#include <utility>
#include <vector>

using namespace std;

template <typename T>
class Table
{
public:
    Table(size_t rows, size_t columns)
    {
        Resize(rows, columns);
    }

    pair<size_t, size_t> Size() const
    {
        return {rows_, columns_};
    }

    void Resize(size_t rows, size_t columns)
    {
        data_.resize(rows, vector<T>(columns));

        for (auto &v : data_)
        {
            v.resize(columns);
        }

        rows_ = rows;
        columns_ = columns;
    }

    auto &operator[](size_t i)
    {
        return data_[i];
    }

    const auto &operator[](size_t i) const
    {
        return data_[i];
    }

private:
    vector<vector<T>> data_;
    size_t rows_;
    size_t columns_;
};

void TestTable()
{
    Table<int> t(1, 1);
    ASSERT_EQUAL(t.Size().first, 1u);
    ASSERT_EQUAL(t.Size().second, 1u);
    t[0][0] = 42;
    ASSERT_EQUAL(t[0][0], 42);
    t.Resize(3, 4);
    ASSERT_EQUAL(t.Size().first, 3u);
    ASSERT_EQUAL(t.Size().second, 4u);
    ASSERT_EQUAL(t[0][0], 42);
    ASSERT_EQUAL(t[2][3], 0);
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestTable);
    return 0;
}
