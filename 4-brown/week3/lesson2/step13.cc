#include "profile.h"
#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <vector>

#pragma warning(disable : 4365)
#pragma warning(disable : 4388)

#pragma warning(disable : 4626)
#pragma warning(disable : 5027)

using namespace std;

template <typename T>
void PrintCoeff(std::ostream &out, int i, const T &coef, bool printed)
{
    bool coeffPrinted = false;
    if (coef == 1 && i > 0)
    {
        out << (printed ? "+" : "");
    }
    else if (coef == -1 && i > 0)
    {
        out << "-";
    }
    else if (coef >= 0 && printed)
    {
        out << "+" << coef;
        coeffPrinted = true;
    }
    else
    {
        out << coef;
        coeffPrinted = true;
    }
    if (i > 0)
    {
        out << (coeffPrinted ? "*" : "") << "x";
    }
    if (i > 1)
    {
        out << "^" << i;
    }
}

template <typename T>
void Shrink(std::vector<T> &v)
{
    while (v.size() > 1 && v.back() == 0)
    {
        v.pop_back();
    }
}

template <typename T>
class Polynomial
{
public:
    class Coeff
    {
    public:
        Coeff &operator=(const T &new_val)
        {
            if (index_ >= coeffs_.size())
            {
                if (new_val != T{})
                {
                    coeffs_.resize(index_ + 1);
                    coeffs_[index_] = new_val;
                }
            }
            else
            {
                coeffs_[index_] = new_val;
                Shrink(coeffs_);
            }

            return *this;
        }

        operator T() const
        {
            return index_ < coeffs_.size() ? coeffs_[index_] : T{};
        }

    private:
        std::vector<T> &coeffs_;
        size_t index_;

        Coeff(std::vector<T> &coeffs, size_t index) : coeffs_(coeffs), index_(index)
        {
        }

        friend class Polynomial<T>;
    };

    Polynomial() = default;
    Polynomial(std::vector<T> coeffs) : coeffs_(std::move(coeffs))
    {
        Shrink(coeffs_);
    }

    template <typename Iterator>
    Polynomial(Iterator first, Iterator last) : coeffs_(first, last)
    {
        Shrink(coeffs_);
    }

    bool operator==(const Polynomial &other) const
    {
        return coeffs_ == other.coeffs_;
    }

    bool operator!=(const Polynomial &other) const
    {
        return !operator==(other);
    }

    int Degree() const
    {
        return static_cast<int>(coeffs_.size()) - 1;
    }

    Polynomial &operator+=(const Polynomial &r)
    {
        if (r.coeffs_.size() > coeffs_.size())
        {
            coeffs_.resize(r.coeffs_.size());
        }
        for (size_t i = 0; i != r.coeffs_.size(); ++i)
        {
            coeffs_[i] += r.coeffs_[i];
        }
        Shrink(coeffs_);
        return *this;
    }

    Polynomial &operator-=(const Polynomial &r)
    {
        if (r.coeffs_.size() > coeffs_.size())
        {
            coeffs_.resize(r.coeffs_.size());
        }
        for (size_t i = 0; i != r.coeffs_.size(); ++i)
        {
            coeffs_[i] -= r.coeffs_[i];
        }
        Shrink(coeffs_);
        return *this;
    }

    T operator[](size_t degree) const
    {
        return degree < coeffs_.size() ? coeffs_[degree] : 0;
    }

    Coeff operator[](size_t degree)
    {
        return Coeff{coeffs_, degree};
    }

    T operator()(const T &x) const
    {
        T res = 0;
        for (auto it = coeffs_.rbegin(); it != coeffs_.rend(); ++it)
        {
            res *= x;
            res += *it;
        }
        return res;
    }

    using const_iterator = typename std::vector<T>::const_iterator;

    const_iterator begin() const
    {
        return coeffs_.cbegin();
    }

    const_iterator end() const
    {
        return coeffs_.cend();
    }

private:
    std::vector<T> coeffs_ = {0};
};

template <typename T>
std::ostream &operator<<(std::ostream &out, const Polynomial<T> &p)
{
    bool printed = false;
    for (int i = p.Degree(); i >= 0; --i)
    {
        if (p[i] != 0)
        {
            PrintCoeff(out, i, p[i], printed);
            printed = true;
        }
    }
    return out;
}

template <typename T>
Polynomial<T> operator+(Polynomial<T> lhs, const Polynomial<T> &rhs)
{
    lhs += rhs;
    return lhs;
}

template <typename T>
Polynomial<T> operator-(Polynomial<T> lhs, const Polynomial<T> &rhs)
{
    lhs -= rhs;
    return lhs;
}

void TestCreation()
{
    {
        Polynomial<int> default_constructed;
        ASSERT_EQUAL(default_constructed.Degree(), 0);
        ASSERT_EQUAL(default_constructed[0], 0);
    }
    {
        Polynomial<double> from_vector({1.0, 2.0, 3.0, 4.0});
        ASSERT_EQUAL(from_vector.Degree(), 3);
        ASSERT_EQUAL(from_vector[0], 1.0);
        ASSERT_EQUAL(from_vector[1], 2.0);
        ASSERT_EQUAL(from_vector[2], 3.0);
        ASSERT_EQUAL(from_vector[3], 4.0);
    }
    {
        const std::vector<int> coeffs = {4, 9, 7, 8, 12};
        Polynomial<int> from_iterators(std::begin(coeffs), std::end(coeffs));
        ASSERT_EQUAL(from_iterators.Degree(), coeffs.size() - 1);
        ASSERT(std::equal(std::cbegin(from_iterators), std::cend(from_iterators), std::begin(coeffs)));
    }
}

void TestEqualComparability()
{
    Polynomial<int> p1({9, 3, 2, 8});
    Polynomial<int> p2({9, 3, 2, 8});
    Polynomial<int> p3({1, 2, 4, 8});

    ASSERT_EQUAL(p1, p2);
    ASSERT(p1 != p3);
}

void TestAddition()
{
    Polynomial<int> p1({9, 3, 2, 8});
    Polynomial<int> p2({1, 2, 4});

    p1 += p2;
    ASSERT_EQUAL(p1, Polynomial<int>({10, 5, 6, 8}));

    auto p3 = p1 + p2;
    ASSERT_EQUAL(p3, Polynomial<int>({11, 7, 10, 8}));

    p3 += Polynomial<int>({-11, 1, -10, -8});
    ASSERT_EQUAL(p3.Degree(), 1);
    const Polynomial<int> expected{{0, 8}};
    ASSERT_EQUAL(p3, expected);
}

void TestSubtraction()
{
    Polynomial<int> p1({9, 3, 2, 8});
    Polynomial<int> p2({1, 2, 4});

    p1 -= p2;
    ASSERT_EQUAL(p1, Polynomial<int>({8, 1, -2, 8}));

    auto p3 = p1 - p2;
    ASSERT_EQUAL(p3, Polynomial<int>({7, -1, -6, 8}));

    p3 -= Polynomial<int>({7, -5, -6, 8});
    ASSERT_EQUAL(p3.Degree(), 1);
    const Polynomial<int> expected{{0, 4}};
    ASSERT_EQUAL(p3, expected);
}

void TestEvaluation()
{
    const Polynomial<int> default_constructed;
    ASSERT_EQUAL(default_constructed(0), 0);
    ASSERT_EQUAL(default_constructed(1), 0);
    ASSERT_EQUAL(default_constructed(-1), 0);
    ASSERT_EQUAL(default_constructed(198632), 0);

    const Polynomial<int64_t> cubic({1, 1, 1, 1});
    ASSERT_EQUAL(cubic(0), 1);
    ASSERT_EQUAL(cubic(1), 4);
    ASSERT_EQUAL(cubic(2), 15);
    ASSERT_EQUAL(cubic(21), 9724);
}

void TestConstAccess()
{
    const Polynomial<int> poly(std::initializer_list<int>{1, 2, 3, 4, 5});

    ASSERT_EQUAL(poly[0], 1);
    ASSERT_EQUAL(poly[1], 2);
    ASSERT_EQUAL(poly[2], 3);
    ASSERT_EQUAL(poly[3], 4);
    ASSERT_EQUAL(poly[4], 5);
    ASSERT_EQUAL(poly[5], 0);
    ASSERT_EQUAL(poly[6], 0);
    ASSERT_EQUAL(poly[608], 0);
}

void TestNonconstAccess()
{
    Polynomial<int> poly;

    poly[0] = 1;
    poly[3] = 12;
    poly[5] = 4;
    ASSERT_EQUAL(poly.Degree(), 5);

    ASSERT_EQUAL(poly[0], 1);
    ASSERT_EQUAL(poly[1], 0);
    ASSERT_EQUAL(poly[2], 0);
    ASSERT_EQUAL(poly[3], 12);
    ASSERT_EQUAL(poly[4], 0);
    ASSERT_EQUAL(poly[5], 4);
    ASSERT_EQUAL(poly[6], 0);
    ASSERT_EQUAL(poly[608], 0);

    ASSERT_EQUAL(poly.Degree(), 5);

    poly[608] = 0;
    ASSERT_EQUAL(poly.Degree(), 5);

    {
        LOG_DURATION("Iteration over const");
        for (size_t i = 10; i < 50000; ++i)
        {
            ASSERT_EQUAL(std::as_const(poly)[i], 0);
            ASSERT_EQUAL(poly.Degree(), 5);
        }
    }
    {
        LOG_DURATION("Iteration over non-const");
        for (size_t i = 10; i < 50000; ++i)
        {
            ASSERT_EQUAL(poly[i], 0);
            ASSERT_EQUAL(poly.Degree(), 5);
        }
    }
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestCreation);
    RUN_TEST(tr, TestEqualComparability);
    RUN_TEST(tr, TestAddition);
    RUN_TEST(tr, TestSubtraction);
    RUN_TEST(tr, TestEvaluation);
    RUN_TEST(tr, TestConstAccess);
    RUN_TEST(tr, TestNonconstAccess);
    return 0;
}
