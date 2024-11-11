#include <exception>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <vector>

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

bool operator==(const Rational &lhs, const Rational &rhs) noexcept
{
    return lhs.Numerator() == rhs.Numerator() &&
           lhs.Denominator() == rhs.Denominator();
}

Rational operator+(const Rational &lhs, const Rational &rhs) noexcept
{
    return {lhs.Numerator() * rhs.Denominator() + rhs.Numerator() * lhs.Denominator(),
            lhs.Denominator() * rhs.Denominator()};
}

Rational operator-(const Rational &lhs, const Rational &rhs) noexcept
{
    return {lhs.Numerator() * rhs.Denominator() - rhs.Numerator() * lhs.Denominator(),
            lhs.Denominator() * rhs.Denominator()};
}

Rational operator*(const Rational &lhs, const Rational &rhs) noexcept
{
    return {lhs.Numerator() * rhs.Numerator(), lhs.Denominator() * rhs.Denominator()};
}

Rational operator/(const Rational &lhs, const Rational &rhs)
{
    if (rhs.Numerator() == 0)
    {
        throw domain_error("division by zero");
    }

    return {lhs.Numerator() * rhs.Denominator(), lhs.Denominator() * rhs.Numerator()};
}

istream &operator>>(istream &is, Rational &r)
{
    int numerator;
    int denominator;

    if ((is >> numerator) &&
        (is.peek() == '/' && is.ignore(1)) &&
        (is >> denominator))
    {
        r = {numerator, denominator};
    }

    return is;
}

ostream &operator<<(ostream &os, const Rational &r)
{
    return os << r.Numerator() << '/' << r.Denominator();
}

bool operator<(const Rational &lhs, const Rational &rhs) noexcept
{
    return lhs.Numerator() * rhs.Denominator() < rhs.Numerator() * lhs.Denominator();
}

int main()
{
    const unordered_map<char, function<Rational(const Rational &, const Rational &)>> op_map =
        {
            {'+', plus<Rational>{}},
            {'-', minus<Rational>{}},
            {'*', multiplies<Rational>{}},
            {'/', divides<Rational>{}},
        };

    try
    {
        Rational r1;
        cin >> r1;
        char op;
        cin >> op;
        Rational r2;
        cin >> r2;
        cout << op_map.at(op)(r1, r2);
    }
    catch (const invalid_argument &)
    {
        cout << "Invalid argument\n";
    }
    catch (const domain_error &)
    {
        cout << "Division by zero\n";
    }
    catch (const exception &ex)
    {
        cout << ex.what() << endl;
    }

    return 0;
}
