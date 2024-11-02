// #include <iostream>

#include <iterator>
#include <string>
#include <vector>

void MoveStrings(std::vector<std::string>& src, std::vector<std::string>& dst)
{
    std::move(src.begin(), src.end(), std::back_inserter(dst));
    src.clear();
}

// int main()
// {
//     std::vector<std::string> source = {"a", "b", "c"};
//     std::vector<std::string> destination = {"z"};

//     MoveStrings(source, destination);

//     // source должен оказаться пустым
//     std::cout << source.size() << std::endl;

//     // destination должен содержать строки "z", "a", "b", "c" именно в таком порядке
//     for (const auto &s : destination)
//     {
//         std::cout << s << std::endl;
//     }

//     return 0;
// }
