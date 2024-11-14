#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

// enum class Lang
// {
//     DE,
//     FR,
//     IT
// };

// struct Region
// {
//     std::string std_name;
//     std::string parent_std_name;
//     std::map<Lang, std::string> names;
//     int64_t population;
// };

int FindMaxRepetitionCount(const std::vector<Region> &regions)
{
    using RegionTuple = std::tuple<const std::string &, const std::string &,
                                   const std::map<Lang, std::string> &, const int64_t &>;

    std::map<RegionTuple, int> repetitions;
    int result = 0;

    for (const auto &[std_name, parent_std_name, names, population] : regions)
    {
        result = std::max(result,
                          ++repetitions[std::tie(std_name, parent_std_name, names, population)]);
    }

    return result;
}

// int main()
// {
//     std::cout << FindMaxRepetitionCount({
//                      {"Moscow",
//                       "Russia",
//                       {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
//                       89},
//                      {"Russia",
//                       "Eurasia",
//                       {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
//                       89},
//                      {"Moscow",
//                       "Russia",
//                       {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
//                       89},
//                      {"Moscow",
//                       "Russia",
//                       {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
//                       89},
//                      {"Russia",
//                       "Eurasia",
//                       {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
//                       89},
//                  })
//               << std::endl;

//     std::cout << FindMaxRepetitionCount({
//                      {"Moscow",
//                       "Russia",
//                       {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
//                       89},
//                      {"Russia",
//                       "Eurasia",
//                       {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
//                       89},
//                      {"Moscow",
//                       "Russia",
//                       {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou deux"}, {Lang::IT, "Mosca"}},
//                       89},
//                      {"Moscow",
//                       "Toulouse",
//                       {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
//                       89},
//                      {"Moscow",
//                       "Russia",
//                       {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
//                       31},
//                  })
//               << std::endl;

//     return 0;
// }
