#include <string>

struct Specialization
{
    std::string str;
    explicit Specialization(std::string &&val) : str(std::move(val)) {}
};

struct Course
{
    std::string str;
    explicit Course(std::string &&val) : str(std::move(val)) {}
};

struct Week
{
    std::string str;
    explicit Week(std::string &&val) : str(std::move(val)) {}
};

struct LectureTitle
{
    std::string specialization;
    std::string course;
    std::string week;

    LectureTitle(Specialization &&spec, Course &&course, Week &&week)
        : specialization(std::move(spec.str)),
          course(std::move(course.str)),
          week(std::move(week.str))
    {
    }
};

// int main()
// {
//     LectureTitle title(
//         Specialization("C++"),
//         Course("White belt"),
//         Week("4th"));

//     return 0;
// }
