#include <fstream>
#include <iomanip>
#include <iostream>

int main()
{
    std::ifstream ifs("input.txt");

    if (!ifs.is_open())
    {
        return 1;
    }

    unsigned n, m;
    ifs >> n >> m;

    for (unsigned i = 0; i < n; i++)
    {
        int v;

        for (unsigned j = 0; j < m; j++)
        {
            ifs >> v;
            ifs.ignore(1);
            std::cout << std::setw(10) << v << (j == m - 1 ? '\n' : ' ');
        }
    }

    ifs.close();

    return 0;
}
