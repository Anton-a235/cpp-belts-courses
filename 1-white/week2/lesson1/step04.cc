// #include <iostream>

#include <string>
#include <algorithm>
#include <iterator>

bool IsPalindrom(const std::string &s)
{
    if (s.length() < 2)
    {
        return true;
    }

    auto mid = s.cbegin();
    std::advance(mid, static_cast<ptrdiff_t>(s.length() / 2));

    return std::equal(s.cbegin(), mid, s.crbegin());
}

// int main()
// {
//     std::string s;

//     std::cin >> s;
//     std::cout << (IsPalindrom(s) ? "true" : "false") << std::endl;

//     return 0;
// }
