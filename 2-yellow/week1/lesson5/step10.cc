#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <type_traits>
#include <utility>
#include <vector>

template <typename T>
inline T Sqr(const T &);

template <typename First, typename Second>
inline std::pair<First, Second> Sqr(const std::pair<First, Second> &);

template <typename Tx>
std::vector<Tx> Sqr(const std::vector<Tx> &);

template <typename Key, typename Val>
std::map<Key, Val> Sqr(const std::map<Key, Val> &);

template <typename T>
inline T Sqr(const T &v)
{
    return v * v;
}

template <typename First, typename Second>
inline std::pair<First, Second> Sqr(const std::pair<First, Second> &v)
{
    return {Sqr(v.first), Sqr(v.second)};
}

template <typename Tx>
std::vector<Tx> Sqr(const std::vector<Tx> &v)
{
    std::vector<Tx> ret;
    for (const auto &el : v)
    {
        ret.push_back(Sqr(el));
    }
    return ret;
}

template <typename Key, typename Val>
std::map<Key, Val> Sqr(const std::map<Key, Val> &v)
{
    std::map<Key, Val> ret;
    for (const auto &[key, val] : v)
    {
        ret[key] = Sqr(val);
    }
    return ret;
}

// int main()
// {
//     auto a = 3.5;
//     std::cerr << "double: " << Sqr(a) << std::endl;

//     std::vector<int> v = {1, 2, 3};
//     std::cerr << "vector: ";
//     for (int x : Sqr(v))
//     {
//         std::cerr << ' ' << x;
//     }
//     std::cerr << std::endl;

//     std::map<int, std::pair<int, int>> map_of_pairs = {
//         {4, {2, 2}},
//         {7, {4, 3}}};
//     std::cerr << "map of pairs:" << std::endl;
//     for (const auto &x : Sqr(map_of_pairs))
//     {
//         std::cerr << x.first << ' ' << x.second.first << ' ' << x.second.second << std::endl;
//     }
//     return 0;
// }
