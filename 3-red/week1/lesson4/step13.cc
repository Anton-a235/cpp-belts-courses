#include <string>
#include <vector>

#pragma warning(disable : 4189)

using namespace std;

#define _CONCAT_(a, b) a##b
#define _UNIQ_ID_(a) _CONCAT_(id, a)
#define UNIQ_ID _UNIQ_ID_(__LINE__)

int main()
{
    int UNIQ_ID = 0;
    string UNIQ_ID = "hello";
    vector<string> UNIQ_ID = {"hello", "world"};
    vector<int> UNIQ_ID = {1, 2, 3, 4};
    return 0;
}
