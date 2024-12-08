#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

void PrintVectorPart(const std::vector<int> &v)
{
    std::for_each(std::reverse_iterator(std::find_if(v.cbegin(), v.cend(),
                                                     [](int n)
                                                     {
                                                         return n < 0;
                                                     })),
                  v.crend(),
                  [](int n)
                  {
                      std::cout << n << " ";
                  });
}

int main()
{
    PrintVectorPart({6, 1, 8, -5, 4});
    std::cout << std::endl;
    PrintVectorPart({-6, 1, 8, -5, 4}); // ничего не выведется
    std::cout << std::endl;
    PrintVectorPart({6, 1, 8, 5, 4});
    std::cout << std::endl;
    return 0;
}
