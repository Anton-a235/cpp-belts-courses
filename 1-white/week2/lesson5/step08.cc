#include <iostream>

#include <unordered_map>
#include <string>
#include <vector>

std::unordered_map<char, int> count_chars(const std::string &word)
{
    std::unordered_map<char, int> result;

    for (const char c : word)
    {
        ++result[c];
    }

    return result;
}

int main()
{
    std::string word1;
    std::string word2;
    int n;

    std::cin >> n;

    for (int i = 0; i < n; i++)
    {
        std::cin >> word1 >> word2;
        std::cout << (count_chars(word1) == count_chars(word2) ? "YES" : "NO")
                  << std::endl;
    }

    return 0;
}
