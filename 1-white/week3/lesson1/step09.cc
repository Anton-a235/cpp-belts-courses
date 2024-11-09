#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

int main()
{
    std::vector<std::string> v;
    unsigned n;

    std::cin >> n;
    v.resize(n);

    for (auto &s : v)
    {
        std::cin >> s;
    }

    std::sort(v.begin(), v.end(),
              [](const std::string &l, const std::string &r)
              {
                  return std::lexicographical_compare(l.cbegin(), l.cend(),
                                                      r.cbegin(), r.cend(),
                                                      [](const int c1, const int c2)
                                                      {
                                                          return std::tolower(c1) <
                                                                 std::tolower(c2);
                                                      });
              });

    for (const auto &s : v)
    {
        std::cout << s << " ";
    }

    return 0;
}
