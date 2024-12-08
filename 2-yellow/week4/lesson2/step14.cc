#include <algorithm>
#include <iostream>
#include <vector>

// enum class Gender
// {
//     FEMALE,
//     MALE
// };

// struct Person
// {
//     int age;          // возраст
//     Gender gender;    // пол
//     bool is_employed; // имеет ли работу
// };

// template <typename InputIt>
// int ComputeMedianAge(InputIt range_begin, InputIt range_end)
// {
//     if (range_begin == range_end)
//     {
//         return 0;
//     }
//     std::vector<typename InputIt::value_type> range_copy(range_begin, range_end);
//     auto middle = std::begin(range_copy) + static_cast<int>(range_copy.size()) / 2;
//     std::nth_element(
//         std::begin(range_copy), middle, std::end(range_copy),
//         [](const Person &lhs, const Person &rhs)
//         {
//             return lhs.age < rhs.age;
//         });
//     return middle->age;
// }

void PrintStats(std::vector<Person> persons)
{
    std::cout << "Median age = "
              << ComputeMedianAge(persons.begin(), persons.end()) << std::endl;

    const auto gender_part = std::partition(persons.begin(), persons.end(),
                                            [](const auto &p)
                                            {
                                                return p.gender == Gender::FEMALE;
                                            });

    std::cout << "Median age for females = "
              << ComputeMedianAge(persons.begin(), gender_part) << std::endl;
    std::cout << "Median age for males = "
              << ComputeMedianAge(gender_part, persons.end()) << std::endl;

    const auto employed_females_part = std::partition(persons.begin(), gender_part,
                                                      [](const auto &p)
                                                      {
                                                          return p.is_employed;
                                                      });

    std::cout << "Median age for employed females = "
              << ComputeMedianAge(persons.begin(), employed_females_part) << std::endl;
    std::cout << "Median age for unemployed females = "
              << ComputeMedianAge(employed_females_part, gender_part) << std::endl;

    const auto employed_males_part = std::partition(gender_part, persons.end(),
                                                    [](const auto &p)
                                                    {
                                                        return p.is_employed;
                                                    });

    std::cout << "Median age for employed males = "
              << ComputeMedianAge(gender_part, employed_males_part) << std::endl;
    std::cout << "Median age for unemployed males = "
              << ComputeMedianAge(employed_males_part, persons.end()) << std::endl;
}

// int main()
// {
//     std::vector<Person> persons = {
//         {31, Gender::MALE, false},
//         {40, Gender::FEMALE, true},
//         {24, Gender::MALE, true},
//         {20, Gender::FEMALE, true},
//         {80, Gender::FEMALE, false},
//         {78, Gender::MALE, false},
//         {10, Gender::FEMALE, false},
//         {55, Gender::MALE, true},
//     };
//     PrintStats(persons);
//     return 0;
// }
