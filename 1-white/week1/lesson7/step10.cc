#include <algorithm>
#include <iostream>
#include <string>

int main()
{
    std::string result;
    int n;

    std::cin >> n;

    while (n > 0)
    {
        result += n % 2 == 0 ? '0' : '1';
        n /= 2;
    }

    std::reverse(result.begin(), result.end());
    std::cout << result << std::endl;

    return 0;
}
