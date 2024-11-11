#include <iostream>
#include <exception>

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

    Rational(int numerator, int denominator)
        : numerator_(numerator),
          denominator_(denominator)
    {
        if (denominator == 0)
        {
            throw invalid_argument("zero denominator");
        }

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

Rational operator/(const Rational &lhs, const Rational &rhs)
{
    if (rhs.Numerator() == 0)
    {
        throw domain_error("division by zero");
    }

    return {lhs.Numerator() * rhs.Denominator(), lhs.Denominator() * rhs.Numerator()};
}

int main()
{
    try
    {
        Rational r(1, 0);
        cout << "Doesn't throw in case of zero denominator" << endl;
        return 1;
    }
    catch (invalid_argument &)
    {
    }

    try
    {
        auto x = Rational(1, 2) / Rational(0, 1);
        cout << "Doesn't throw in case of division by zero" << endl;
        return 2;
    }
    catch (domain_error &)
    {
    }

    cout << "OK" << endl;
    return 0;
}
