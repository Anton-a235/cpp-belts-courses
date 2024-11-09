// #include <iostream>

#include <algorithm>
#include <string>

class ReversibleString
{
public:
    ReversibleString() = default;

    ReversibleString(const std::string &str)
        : str_(str)
    {
    }

    ReversibleString(std::string &&str) noexcept
        : str_(std::move(str))
    {
    }

    void Reverse()
    {
        std::reverse(str_.begin(), str_.end());
    }

    const std::string &ToString() const noexcept
    {
        return str_;
    }

private:
    std::string str_;
};

// int main()
// {
//     ReversibleString s("live");
//     s.Reverse();
//     std::cout << s.ToString() << std::endl;

//     s.Reverse();
//     const ReversibleString &s_ref = s;
//     std::string tmp = s_ref.ToString();
//     std::cout << tmp << std::endl;

//     ReversibleString empty;
//     std::cout << '"' << empty.ToString() << '"' << std::endl;

//     return 0;
// }
