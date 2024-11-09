// #include <iostream>

#include <algorithm>
#include <string>
#include <vector>

class SortedStrings
{
public:
    void AddString(const std::string &s)
    {
        strings_.push_back(s);
        std::sort(strings_.begin(), strings_.end());
    }

    const std::vector<std::string> &GetSortedStrings() const noexcept
    {
        return strings_;
    }

private:
    std::vector<std::string> strings_;
};

// void PrintSortedStrings(SortedStrings &strings)
// {
//     for (const std::string &s : strings.GetSortedStrings())
//     {
//         std::cout << s << " ";
//     }

//     std::cout << std::endl;
// }

// int main()
// {
//     SortedStrings strings;

//     strings.AddString("first");
//     strings.AddString("third");
//     strings.AddString("second");
//     PrintSortedStrings(strings);

//     strings.AddString("second");
//     PrintSortedStrings(strings);

//     return 0;
// }
