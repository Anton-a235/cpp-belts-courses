#include <algorithm>
#include <ctime>
#include <exception>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <unordered_map>
#include <utility>

class Date
{
public:
    Date() noexcept
        : ts_(0)
    {
    }

    explicit Date(std::tm time)
    {
        time.tm_sec = time.tm_min = time.tm_hour = time.tm_isdst = 0;

        if ((ts_ = std::mktime(&time)) < 0)
            throw std::invalid_argument("invalid time");
    }

    Date(const Date& other) noexcept = default;
    Date& operator=(const Date& other) noexcept = default;

    inline std::time_t timestamp() const noexcept
    {
        return ts_;
    }

    Date& operator++() noexcept
    {
        ts_ += 3600 * 24;
        return *this;
    }

    Date operator++(int) noexcept
    {
        Date tmp(*this);
        ++(*this);
        return tmp;
    }

    static long long days_count(const Date& from, const Date& to_inclusive) noexcept
    {
        return to_inclusive.ts_ < from.ts_ ? 0 : (to_inclusive.ts_ - from.ts_) / 3600 / 24 + 1;
    }

private:
    std::time_t ts_;
};

inline bool operator<(const Date& lhs, const Date& rhs) noexcept
{
    return lhs.timestamp() < rhs.timestamp();
}

inline bool operator<=(const Date& lhs, const Date& rhs) noexcept
{
    return lhs.timestamp() <= rhs.timestamp();
}

std::istream& operator>>(std::istream& is, Date& date)
{
    std::tm tm{};
    is >> std::get_time(&tm, "%Y-%m-%d");
    if (is)
        date = Date(std::move(tm));
    return is;
}

using Budget = std::map<Date, double>;
using Result = std::optional<double>;

class Request
{
public:
    virtual ~Request() = default;

    Request(Date from, Date to, double value = 0.0)
        : from_(std::move(from)), to_(std::move(to)), value_(value)
    {
    }

    virtual Result process(Budget& budget) const = 0;

protected:
    Date from_;
    Date to_;
    double value_;
};

using RequestPtr = std::unique_ptr<Request>;

namespace Requests
{

class ComputeIncome : public Request
{
public:
    using Request::Request;

    ComputeIncome() = delete;
    ~ComputeIncome() override = default;

    Result process(Budget& budget) const override
    {
        const auto from_it = budget.lower_bound(from_);
        const auto to_it = budget.upper_bound(to_);

        return std::accumulate(from_it,
                               to_it,
                               0.0,
                               [](double sum, const auto& p)
                               {
                                   return sum + p.second;
                               });
    }
};

class Earn : public Request
{
public:
    using Request::Request;

    Earn() = delete;
    ~Earn() override = default;

    Result process(Budget& budget) const override
    {
        const auto days = Date::days_count(from_, to_);
        const double earn_per_day = value_ / static_cast<double>(days);

        for (Date cur_day = from_; cur_day <= to_; cur_day = ++cur_day)
            budget[cur_day] += earn_per_day;

        return std::nullopt;
    }
};

class PayTax : public Request
{
public:
    using Request::Request;

    PayTax() = delete;
    ~PayTax() override = default;

    Result process(Budget& budget) const override
    {
        const auto from_it = budget.lower_bound(from_);
        const auto to_it = budget.upper_bound(to_);

        std::for_each(from_it,
                      to_it,
                      [](auto& p)
                      {
                          p.second *= 0.87;
                      });

        return std::nullopt;
    }
};

enum Type
{
    COMPUTE_INCOME,
    EARN,
    PAY_TAX
};

Type type_from_command(const std::string& command)
{
    // clang-format off
    static const std::unordered_map<std::string, Type> mapping
    {
        {"ComputeIncome", Type::COMPUTE_INCOME},
        {"Earn",          Type::EARN},
        {"PayTax",        Type::PAY_TAX}
    };
    // clang-format on

    return mapping.at(command);
}

class Builder
{
public:
    Builder& set_type(Type type) noexcept
    {
        type_ = type;
        return *this;
    }

    Builder& set_from(Date from) noexcept
    {
        from_ = std::move(from);
        return *this;
    }

    Builder& set_to(Date to) noexcept
    {
        to_ = std::move(to);
        return *this;
    }

    Builder& set_value(double value) noexcept
    {
        value_ = value;
        return *this;
    }

    RequestPtr build()
    {
        if (!type_)
            throw std::invalid_argument("type not set");
        if (to_ < from_)
            throw std::invalid_argument("dates to < from");
        if (!value_ && *type_ == Type::EARN)
            throw std::invalid_argument("value not set for Earn request");

        switch (*type_)
        {
        case Type::COMPUTE_INCOME:
            return std::make_unique<ComputeIncome>(std::move(from_), std::move(to_));
        case Type::EARN:
            return std::make_unique<Earn>(std::move(from_), std::move(to_), *value_);
        case Type::PAY_TAX:
            return std::make_unique<PayTax>(std::move(from_), std::move(to_));
        default:
            return nullptr;
        }
    }

private:
    std::optional<Type> type_;

    Date from_;
    Date to_;
    std::optional<double> value_;
};

RequestPtr read_one(std::istream& is)
{
    std::string command;
    Requests::Type type;
    Date from;
    Date to;
    double value = 0.0;

    if (!(is >> command >> from >> to))
        return nullptr;

    if (type = Requests::type_from_command(command); type == Requests::Type::EARN)
        if (!(is >> value))
            return nullptr;

    // clang-format off
    return Requests::Builder()
        .set_type(type)
        .set_from(std::move(from))
        .set_to(std::move(to))
        .set_value(value)
        .build();
    // clang-format on
}

} // namespace Requests

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.precision(25);

    Budget budget;
    size_t n;
    std::cin >> n;

    while (n--)
    {
        const auto request = Requests::read_one(std::cin);

        if (!request)
            return 1;

        const auto result = request->process(budget);

        if (result)
            std::cout << *result << '\n';
    }

    return 0;
}
