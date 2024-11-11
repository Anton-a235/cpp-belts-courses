#include <cmath>
#include <iostream>

using namespace std;

int gcd(int a, int b) noexcept
{
    if (b == 0)
    {
        return a;
    }

    return gcd(b, a % b);
}

class Rational
{
public:
    Rational() noexcept : Rational(0, 1)
    {
    }

    Rational(int numerator, int denominator) noexcept
        : numerator_(numerator),
          denominator_(denominator)
    {
        reduce_();
    }

    inline int Numerator() const noexcept
    {
        return numerator_;
    }

    inline int Denominator() const noexcept
    {
        return denominator_;
    }

private:
    int numerator_;
    int denominator_;

    void reduce_() noexcept
    {
        if (numerator_ == 0)
        {
            denominator_ = 1;
            return;
        }

        if (denominator_ < 0)
        {
            numerator_ *= -1;
            denominator_ *= -1;
        }

        const int d = gcd(abs(numerator_), denominator_);
        numerator_ /= d;
        denominator_ /= d;
    }
};

bool operator==(const Rational &lhs, const Rational &rhs)
{
    return lhs.Numerator() == rhs.Numerator() &&
           lhs.Denominator() == rhs.Denominator();
}

Rational operator+(const Rational &lhs, const Rational &rhs)
{
    return {lhs.Numerator() * rhs.Denominator() + rhs.Numerator() * lhs.Denominator(),
            lhs.Denominator() * rhs.Denominator()};
}

Rational operator-(const Rational &lhs, const Rational &rhs)
{
    return {lhs.Numerator() * rhs.Denominator() - rhs.Numerator() * lhs.Denominator(),
            lhs.Denominator() * rhs.Denominator()};
}

int main()
{
    {
        Rational r1(4, 6);
        Rational r2(2, 3);
        bool equal = r1 == r2;
        if (!equal)
        {
            cout << "4/6 != 2/3" << endl;
            return 1;
        }
    }

    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a + b;
        bool equal = c == Rational(2, 1);
        if (!equal)
        {
            cout << "2/3 + 4/3 != 2" << endl;
            return 2;
        }
    }

    {
        Rational a(5, 7);
        Rational b(2, 9);
        Rational c = a - b;
        bool equal = c == Rational(31, 63);
        if (!equal)
        {
            cout << "5/7 - 2/9 != 31/63" << endl;
            return 3;
        }
    }

    cout << "OK" << endl;
    return 0;
}
