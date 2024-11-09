#include <iostream>
#include <string>
#include <vector>

struct Student
{
    std::string first_name;
    std::string last_name;
    unsigned b_day;
    unsigned b_month;
    unsigned b_year;
};

int main()
{
    unsigned n;
    std::cin >> n;

    std::vector<Student> v(n);

    for (auto &st : v)
    {
        std::cin >> st.first_name >> st.last_name >> st.b_day >> st.b_month >> st.b_year;
    }

    std::cin >> n;

    while (n--)
    {
        std::string req;
        unsigned k;

        std::cin >> req >> k;
        k--;

        if (k >= v.size())
        {
            std::cout << "bad request\n";
            continue;
        }

        if (req.compare("name") == 0)
        {
            std::cout << v[k].first_name << ' ' << v[k].last_name << std::endl;
        }
        else if (req.compare("date") == 0)
        {
            std::cout << v[k].b_day << '.' << v[k].b_month << '.' << v[k].b_year << std::endl;
        }
        else
        {
            std::cout << "bad request\n";
        }
    }

    return 0;
}
