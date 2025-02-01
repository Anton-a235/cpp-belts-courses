#include "test_runner.h"

#include <algorithm>
#include <future>
#include <iterator>
#include <numeric>
#include <vector>

using namespace std;

int64_t CalculateMatrixSum(const vector<vector<int>> &matrix)
{
    constexpr int thread_count = 4;

    vector<future<int64_t>> futures;
    futures.reserve(thread_count);

    for (int i = 0; i < thread_count; ++i)
    {
        size_t from = matrix.size() * i / thread_count;
        size_t to = matrix.size() * (i + 1) / thread_count;

        futures.push_back(
            async([from, to, &matrix]
                  {
                      int64_t sum = 0;
                      for (size_t row = from; row < to; ++row)
                      {
                          for (const auto& a: matrix[row])
                          {
                              sum += a;
                          }
                      }
                      return sum;
                  }));
    }

    return accumulate(futures.begin(), futures.end(), 0ll,
                      [](int64_t sum, future<int64_t> &f)
                      {
                          return sum + f.get();
                      });
}

void TestCalculateMatrixSum()
{
    const vector<vector<int>> matrix = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}};
    ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestCalculateMatrixSum);
}
