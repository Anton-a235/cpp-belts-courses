#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

using namespace std;

class Matrix
{
public:
    Matrix() : num_rows_{}, num_cols_{}
    {
    }

    Matrix(int num_rows, int num_cols)
    {
        Reset(num_rows, num_cols);
    }

    void Reset(int num_rows, int num_cols)
    {
        if (num_rows < 0 || num_cols < 0)
        {
            throw out_of_range("negative dimension value");
        }

        if (num_rows * num_cols == 0)
        {
            num_rows_ = 0;
            num_cols_ = 0;
            data_.clear();
        }
        else
        {
            num_rows_ = num_rows;
            num_cols_ = num_cols;
            data_.assign(static_cast<size_t>(num_rows * num_cols), 0);
        }
    }

    inline int At(int row, int col) const
    {
        check_range(row, col);
        return data_.at(static_cast<size_t>(row * num_cols_ + col));
    }

    inline int &At(int row, int col)
    {
        check_range(row, col);
        return data_.at(static_cast<size_t>(row * num_cols_ + col));
    }

    inline int GetNumRows() const noexcept
    {
        return num_rows_;
    }

    inline int GetNumColumns() const noexcept
    {
        return num_cols_;
    }

    inline bool Compare(const Matrix &other) const
    {
        return num_rows_ == other.num_rows_ &&
               num_cols_ == other.num_cols_ &&
               data_ == other.data_;
    }

    inline Matrix &operator+=(const Matrix &other)
    {
        if (num_rows_ != other.num_rows_ ||
            num_cols_ != other.num_cols_)
        {
            throw invalid_argument(
                string{"this->data_.size() = "} + to_string(this->data_.size()) +
                ", this->num_rows_ = " + to_string(this->num_rows_) +
                ", this->num_cols_ = " + to_string(this->num_cols_) +
                ", other.data_.size() = " + to_string(other.data_.size()) +
                ", other.num_rows_ = " + to_string(other.num_rows_) +
                ", other.num_cols_ = " + to_string(other.num_cols_));
        }

        vector<int> result;
        transform(data_.cbegin(), data_.cend(), other.data_.cbegin(),
                  back_inserter(result), plus<int>{});

        data_ = move(result);
        return *this;
    }

private:
    vector<int> data_;
    int num_rows_;
    int num_cols_;

    inline void check_range(int row, int col) const
    {
        if (row < 0 || col < 0 ||
            row >= num_rows_ || col >= num_cols_)
        {
            throw out_of_range("index is out of range");
        }
    }
};

istream &operator>>(istream &is, Matrix &m)
{
    int num_rows;
    int num_cols;

    if (is >> num_rows >> num_cols)
    {
        Matrix result(num_rows, num_cols);

        for (int i = 0; i < num_rows; i++)
        {
            for (int j = 0; j < num_cols; j++)
            {
                if (!(is >> result.At(i, j)))
                {
                    return is;
                }
            }
        }

        m = move(result);
    }

    return is;
}

ostream &operator<<(ostream &os, const Matrix &m)
{
    os << m.GetNumRows() << ' ' << m.GetNumColumns() << endl;

    if (m.GetNumRows() == 0 || m.GetNumColumns() == 0)
    {
        return os;
    }

    for (int i = 0; i < m.GetNumRows(); i++)
    {
        os << m.At(i, 0);

        for (int j = 1; j < m.GetNumColumns(); j++)
        {
            os << ' ' << m.At(i, j);
        }

        os << endl;
    }

    return os;
}

inline bool operator==(const Matrix &lhs, const Matrix &rhs)
{
    return lhs.Compare(rhs);
}

inline Matrix operator+(const Matrix &lhs, const Matrix &rhs)
{
    Matrix result(lhs);
    result += rhs;
    return result;
}

// int main()
// {
//     {
//         Matrix one;
//         Matrix two;

//         cin >> one >> two;
//         cout << one + two << endl;

//         one.Reset(one.GetNumRows() + 1, one.GetNumColumns() + 1);
//         cout << one << endl;
//         cout << "one == Matrix(one): " << (one == Matrix(one)) << endl;
//     }

//     {
//         Matrix one(3, 0);
//         Matrix two(0, 5);

//         cout << "one == two: " << (one == two) << endl;
//         cout << one + two << endl;
//     }

//     return 0;
// }
