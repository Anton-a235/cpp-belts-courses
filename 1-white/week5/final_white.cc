#include <exception>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

class Date
{
public:
    Date() noexcept : Date(1, 1, 1) {}

    Date(int year, int month, int day)
        : year_(year), month_(month), day_(day)
    {
        if (month < 1 || month > 12)
        {
            throw std::invalid_argument(
                std::string{"Month value is invalid: "}.append(std::to_string(month)));
        }

        if (day < 1 || day > 31)
        {
            throw std::invalid_argument(
                std::string{"Day value is invalid: "}.append(std::to_string(day)));
        }
    }

    inline int GetYear() const noexcept
    {
        return year_;
    }

    inline int GetMonth() const noexcept
    {
        return month_;
    }

    inline int GetDay() const noexcept
    {
        return day_;
    }

private:
    int year_;
    int month_;
    int day_;
};

inline bool operator<(const Date &lhs, const Date &rhs) noexcept
{
    if (lhs.GetYear() != rhs.GetYear())
        return lhs.GetYear() < rhs.GetYear();
    if (lhs.GetMonth() != rhs.GetMonth())
        return lhs.GetMonth() < rhs.GetMonth();
    return lhs.GetDay() < rhs.GetDay();
}

inline std::istream &operator>>(std::istream &is, Date &date)
{
    int year;
    int month;
    int day;

    if ((is >> year) &&
        (is.peek() == '-' && is.ignore(1)) &&
        (is >> month) &&
        (is.peek() == '-' && is.ignore(1)) &&
        (is >> day))
    {
        date = {year, month, day};
    }

    return is;
}

inline std::ostream &operator<<(std::ostream &os, const Date &date)
{
    return os << std::setw(4) << std::setfill('0') << date.GetYear() << "-"
              << std::setw(2) << std::setfill('0') << date.GetMonth() << "-"
              << std::setw(2) << std::setfill('0') << date.GetDay();
}

class Database
{
public:
    void AddEvent(const Date &date, const std::string &event)
    {
        events_[date].insert(event);
    }

    bool DeleteEvent(const Date &date, const std::string &event)
    {
        return events_[date].erase(event) > 0;
    }

    size_t DeleteDate(const Date &date)
    {
        const auto it = events_.find(date);

        if (it == events_.cend())
        {
            return 0;
        }

        const auto res = it->second.size();
        events_.erase(it);
        return res;
    }

    inline const std::set<std::string> &Find(const Date &date) const
    {
        return events_.at(date);
    }

    void Print() const
    {
        for (const auto &[date, events] : events_)
        {
            for (const auto &event : events)
            {
                std::cout << date << " " << event << std::endl;
            }
        }
    }

private:
    std::map<Date, std::set<std::string>> events_;
};

Date ParseDate(const std::string &date_str)
{
    std::stringstream ss(date_str);
    Date date;

    if (!(ss >> date) || !ss.eof())
    {
        throw std::runtime_error(std::string{"Wrong date format: "}.append(date_str));
    }

    return date;
}

int main()
{
    Database db;
    std::string command_line;

    while (std::getline(std::cin, command_line))
    {
        std::stringstream ss(std::move(command_line));
        std::string command;
        std::string date_str;
        std::string event;
        Date date;

        if (!(ss >> command))
        {
            // Ignore empty command
            continue;
        }

        try
        {
            if (command == "Add")
            {
                ss >> date_str >> event;
                date = ParseDate(date_str);
                db.AddEvent(date, event);
            }
            else if (command == "Del")
            {
                ss >> date_str;
                date = ParseDate(date_str);

                if (ss >> event)
                {
                    if (db.DeleteEvent(date, event))
                    {
                        std::cout << "Deleted successfully\n";
                    }
                    else
                    {
                        std::cout << "Event not found\n";
                    }
                }
                else
                {
                    std::cout << "Deleted " << db.DeleteDate(date) << " events\n";
                }
            }
            else if (command == "Find")
            {
                ss >> date_str;
                date = ParseDate(date_str);

                const auto &events = db.Find(date);

                for (const auto &event_ : events)
                {
                    std::cout << event_ << std::endl;
                }
            }
            else if (command == "Print")
            {
                db.Print();
            }
            else
            {
                std::cout << "Unknown command: " << command << std::endl;
                break;
            }
        }
        catch (const std::out_of_range &)
        {
            // thrown by db.Find(), found nothing
        }
        catch (const std::exception &ex)
        {
            std::cout << ex.what() << std::endl;
            break;
        }
    }

    return 0;
}
