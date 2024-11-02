// #include <iostream>

#include <algorithm>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

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

std::vector<std::string> PalindromFilter(const std::vector<std::string> &words, int minLength)
{
    std::vector<std::string> result;

    std::copy_if(words.cbegin(), words.cend(), std::back_inserter(result),
                 [minLength](const std::string &word)
                 {
                     return static_cast<int>(word.length()) >= minLength && IsPalindrom(word);
                 });

    return result;
}

// int main()
// {
//     std::vector<std::string> words;
//     std::string word;
//     int minLength;

//     std::cin >> minLength;

//     while (std::cin >> word)
//     {
//         words.push_back(std::move(word));
//     }

//     std::vector<std::string> result = PalindromFilter(words, minLength);

//     for (const auto &w : result)
//     {
//         std::cout << w << std::endl;
//     }

//     return 0;
// }
