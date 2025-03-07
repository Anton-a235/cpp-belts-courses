#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream &operator<<(ostream &os, const vector<T> &s)
{
    os << "{";
    bool first = true;
    for (const auto &x : s)
    {
        if (!first)
        {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class T>
ostream &operator<<(ostream &os, const set<T> &s)
{
    os << "{";
    bool first = true;
    for (const auto &x : s)
    {
        if (!first)
        {
            os << ", ";
        }
        first = false;
        os << x;
    }
    return os << "}";
}

template <class K, class V>
ostream &operator<<(ostream &os, const map<K, V> &m)
{
    os << "{";
    bool first = true;
    for (const auto &kv : m)
    {
        if (!first)
        {
            os << ", ";
        }
        first = false;
        os << kv.first << ": " << kv.second;
    }
    return os << "}";
}

template <class T, class U>
void AssertEqual(const T &t, const U &u, const string &hint = {})
{
    if (t != u)
    {
        ostringstream os;
        os << "Assertion failed: " << t << " != " << u;
        if (!hint.empty())
        {
            os << " hint: " << hint;
        }
        throw runtime_error(os.str());
    }
}

void Assert(bool b, const string &hint)
{
    AssertEqual(b, true, hint);
}

class TestRunner
{
public:
    template <class TestFunc>
    void RunTest(TestFunc func, const string &test_name)
    {
        try
        {
            func();
            cerr << test_name << " OK" << endl;
        }
        catch (exception &e)
        {
            ++fail_count;
            cerr << test_name << " fail: " << e.what() << endl;
        }
        catch (...)
        {
            ++fail_count;
            cerr << "Unknown exception caught" << endl;
        }
    }

    ~TestRunner()
    {
        if (fail_count > 0)
        {
            cerr << fail_count << " unit tests failed. Terminate" << endl;
            exit(1);
        }
    }

private:
    int fail_count = 0;
};

// class Person
// {
// public:
//     // Вы можете вставлять сюда различные реализации класса,
//     // чтобы проверить, что ваши тесты пропускают корректный код
//     // и ловят некорректный
//     void ChangeFirstName(int, const string &)
//     {
//     }
//     void ChangeLastName(int, const string &)
//     {
//     }
//     string GetFullName(int)
//     {
//         return {};
//     }
// };

void PersonTest()
{
    Person p;
    AssertEqual(p.GetFullName(2000), "Incognito");
    p.ChangeFirstName(2001, "First1");
    AssertEqual(p.GetFullName(2000), "Incognito");
    AssertEqual(p.GetFullName(2001), "First1 with unknown last name");
    AssertEqual(p.GetFullName(2002), "First1 with unknown last name");
    p.ChangeLastName(2002, "Last1");
    AssertEqual(p.GetFullName(2000), "Incognito");
    AssertEqual(p.GetFullName(2001), "First1 with unknown last name");
    AssertEqual(p.GetFullName(2002), "First1 Last1");
    p.ChangeLastName(2000, "Last2");
    AssertEqual(p.GetFullName(1999), "Incognito");
    AssertEqual(p.GetFullName(2000), "Last2 with unknown first name");
    AssertEqual(p.GetFullName(2001), "First1 Last2");
}

int main()
{
    TestRunner runner;
    runner.RunTest(PersonTest, "PersonTest");
    return 0;
}
