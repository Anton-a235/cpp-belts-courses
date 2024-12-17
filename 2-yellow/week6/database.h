#pragma once

#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "date.h"

using EventPred = std::function<bool(const Date &, const std::string &)>;

class Database
{
public:
    void Add(const Date &date, const std::string &event);
    size_t RemoveIf(EventPred pred);
    void Print(std::ostream &os) const;
    std::string Last(const Date &date) const;
    std::vector<std::string> FindIf(EventPred pred) const;

private:
    std::map<Date, std::vector<std::string>> events_;
    std::map<Date, std::set<std::string>> events_set_;
};
