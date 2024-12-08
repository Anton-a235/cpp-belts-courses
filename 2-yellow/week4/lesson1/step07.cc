#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

std::vector<std::string> SplitIntoWords(const std::string_view &s)
{
    std::vector<std::string> result;
    auto it_from = s.cbegin();
    auto it_to = it_from;

    while (it_from != s.cend())
    {
        it_to = std::find(it_from, s.cend(), ' ');
        result.emplace_back(it_from, it_to);
        it_from = it_to != s.cend() ? ++it_to : it_to;
    }

    return result;
}

int main()
{
    std::string s = "C Cpp Java Python";

    std::vector<std::string> words = SplitIntoWords(s);
    std::cout << words.size() << " ";
    for (auto it = std::begin(words); it != std::end(words); ++it)
    {
        if (it != std::begin(words))
        {
            std::cout << "/";
        }
        std::cout << *it;
    }
    std::cout << std::endl;

    return 0;
}
