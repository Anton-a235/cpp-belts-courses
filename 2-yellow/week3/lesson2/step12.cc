#include "sum_reverse_sort.h"

#include <algorithm>
#include <type_traits>

using namespace std;

int Sum(int x, int y)
{
    return x + y;
}

string Reverse(string s)
{
    reverse(s.begin(), s.end());
    return s;
}

void Sort(vector<int> &nums)
{
    sort(nums.begin(), nums.end());
}

// #include <iostream>

// int main()
// {
//     cout << Sum(2, 3) << endl;

//     const auto str = Reverse("qwerty");
//     cout << str << endl;

//     vector<int> nums = {3, 1, 2};
//     Sort(nums);

//     for (const auto &n : nums)
//     {
//         cout << n << " ";
//     }

//     return 0;
// }
