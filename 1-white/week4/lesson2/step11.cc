#include <fstream>
#include <string>

int main()
{
    fclose(stdout);

    std::ifstream ifs("input.txt");
    std::ofstream ofs("output.txt");

    if (!ifs.is_open() || !ofs.is_open())
    {
        return 1;
    }

    std::string line;

    while (std::getline(ifs, line))
    {
        ofs << line << std::endl;
    }

    ifs.close();
    ofs.close();

    return 0;
}
