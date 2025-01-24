#include "test_runner.h"

#include <ostream>
using namespace std;

#define PRINT_VALUES(out, x, y) (out) << (x) << endl \
                                      << (y) << endl

int main()
{
    TestRunner tr;
    tr.RunTest([]
               {
                   ostringstream output;
                   PRINT_VALUES(output, 5, "red belt");
                   ASSERT_EQUAL(output.str(), "5\nred belt\n");
               },
               "PRINT_VALUES usage example");

    ostringstream output;
    if (false)
    {
        PRINT_VALUES(output, 1, "red belt 1");
    }
    else
    {
        PRINT_VALUES(output, 2, "red belt 2");
    }
    cout << output.str();
}
