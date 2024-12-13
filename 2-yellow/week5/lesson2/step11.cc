#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

constexpr double pi = 3.14;

class Figure
{
public:
    virtual ~Figure() = default;

    virtual const string &Name() = 0;
    virtual double Perimeter() = 0;
    virtual double Area() = 0;
};

class Triangle : public Figure
{
public:
    Triangle(double a, double b, double c)
        : a_(a), b_(b), c_(c)
    {
    }

    const string &Name()
    {
        return name_;
    }

    double Perimeter()
    {
        return a_ + b_ + c_;
    }

    double Area()
    {
        const auto p = Perimeter() / 2;
        return sqrt(p * (p - a_) * (p - b_) * (p - c_));
    }

private:
    const static string name_;

    double a_;
    double b_;
    double c_;
};

const string Triangle::name_ = "TRIANGLE";

class Rect : public Figure
{
public:
    Rect(double a, double b)
        : a_(a), b_(b)
    {
    }

    const string &Name()
    {
        return name_;
    }

    double Perimeter()
    {
        return 2 * (a_ + b_);
    }

    double Area()
    {
        return a_ * b_;
    }

private:
    const static string name_;

    double a_;
    double b_;
};

const string Rect::name_ = "RECT";

class Circle : public Figure
{
public:
    Circle(double r)
        : r_(r)
    {
    }

    const string &Name()
    {
        return name_;
    }

    double Perimeter()
    {
        return 2 * pi * r_;
    }

    double Area()
    {
        return pi * r_ * r_;
    }

private:
    const static string name_;

    double r_;
};

const string Circle::name_ = "CIRCLE";

shared_ptr<Figure> CreateFigure(istream &is)
{
    string type;
    is >> type;

    double a;
    double b;
    double c;

    if (type == "TRIANGLE")
    {
        is >> a >> b >> c;
        return make_shared<Triangle>(a, b, c);
    }
    else if (type == "RECT")
    {
        is >> a >> b;
        return make_shared<Rect>(a, b);
    }
    else if (type == "CIRCLE")
    {
        is >> a;
        return make_shared<Circle>(a);
    }

    return nullptr;
}

int main()
{
    vector<shared_ptr<Figure>> figures;
    for (string line; getline(cin, line);)
    {
        istringstream is(line);

        string command;
        is >> command;
        if (command == "ADD")
        {
            figures.push_back(CreateFigure(is));
        }
        else if (command == "PRINT")
        {
            for (const auto &current_figure : figures)
            {
                cout << fixed << setprecision(3)
                     << current_figure->Name() << " "
                     << current_figure->Perimeter() << " "
                     << current_figure->Area() << endl;
            }
        }
    }
    return 0;
}
