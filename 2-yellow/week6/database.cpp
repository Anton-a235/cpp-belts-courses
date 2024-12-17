#include "database.h"
#include "date.h"

#include <algorithm>
#include <exception>
#include <functional>
#include <iterator>

void Database::Add(const Date &date, const std::string &event)
{
    if (events_set_.count(date) == 0 ||
        events_set_[date].count(event) == 0)
    {
        events_[date].push_back(event);
        events_set_[date].insert(event);
    }
}

size_t Database::RemoveIf(EventPred pred)
{
    std::vector<Date> empty_dates;
    size_t removed_count = 0;

    for (auto &[date, events] : events_)
    {
        auto old_size = events.size();
        const auto partition_pred = std::bind(std::not_fn(pred), std::cref(date), std::placeholders::_1);
        const auto part_begin = std::stable_partition(events.begin(), events.end(), partition_pred);

        for (auto it_ = part_begin; it_ != events.end(); ++it_)
        {
            events_set_[date].erase(*it_);
        }

        events.erase(part_begin, events.end());
        removed_count += old_size - events.size();

        if (events.empty())
        {
            empty_dates.push_back(date);
        }
    }

    for (const auto &date : empty_dates)
    {
        events_.erase(date);
        events_set_.erase(date);
    }

    return removed_count;
}

void Database::Print(std::ostream &os) const
{
    for (const auto &[date, events] : events_)
    {
        for (const auto &event : events)
        {
            os << date << ' ' << event << std::endl;
        }
    }
}

std::string Database::Last(const Date &date) const
{
    auto date_it = events_.upper_bound(date);

    if (date_it == events_.cbegin())
    {
        throw std::invalid_argument("");
    }

    date_it = std::prev(date_it);

    std::ostringstream ss;
    ss << date_it->first << ' ' << date_it->second.back();
    return ss.str();
}

std::vector<std::string> Database::FindIf(EventPred pred) const
{
    std::vector<std::string> result;

    for (const auto &[date, events] : events_)
    {
        for (const auto &event : events)
        {
            if (pred(date, event))
            {
                std::ostringstream ss;
                ss << date << ' ' << event;
                result.push_back(ss.str());
            }
        }
    }

    return result;
}
