#include <array>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <utility>

#pragma warning(disable : 4244)
#pragma warning(disable : 4365)
#pragma warning(disable : 5219)

using namespace std;

pair<string_view, optional<string_view>> SplitTwoStrict(string_view s, string_view delimiter = " ")
{
    const size_t pos = s.find(delimiter);
    if (pos == s.npos)
    {
        return {s, nullopt};
    }
    else
    {
        return {s.substr(0, pos), s.substr(pos + delimiter.length())};
    }
}

pair<string_view, string_view> SplitTwo(string_view s, string_view delimiter = " ")
{
    const auto [lhs, rhs_opt] = SplitTwoStrict(s, delimiter);
    return {lhs, rhs_opt.value_or("")};
}

string_view ReadToken(string_view& s, string_view delimiter = " ")
{
    const auto [lhs, rhs] = SplitTwo(s, delimiter);
    s = rhs;
    return lhs;
}

int ConvertToInt(string_view str)
{
    // use std::from_chars when available to git rid of string copy
    size_t pos;
    const int result = stoi(string(str), &pos);
    if (pos != str.length())
    {
        std::stringstream error;
        error << "string " << str << " contains " << (str.length() - pos) << " trailing chars";
        throw invalid_argument(error.str());
    }
    return result;
}

template <typename Number>
void ValidateBounds(Number number_to_check, Number min_value, Number max_value)
{
    if (number_to_check < min_value || number_to_check > max_value)
    {
        std::stringstream error;
        error << number_to_check << " is out of [" << min_value << ", " << max_value << "]";
        throw out_of_range(error.str());
    }
}

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

    static Date from_string(const std::string& date)
    {
        std::tm tm{};
        std::istringstream ss(date);
        ss >> std::get_time(&tm, "%Y-%m-%d");
        if (!ss)
            throw std::invalid_argument("invalid date");
        return Date(std::move(tm));
    }

    static long long days_count(const Date& from, const Date& to) noexcept
    {
        return to.ts_ < from.ts_ ? 0 : (to.ts_ - from.ts_) / 3600 / 24;
    }

private:
    std::time_t ts_;
};

static const Date START_DATE = Date::from_string("2000-01-01");
static const Date END_DATE = Date::from_string("2100-01-01");
static const size_t DAY_COUNT = Date::days_count(START_DATE, END_DATE);
static const size_t DAY_COUNT_P2 = 1 << 16;
static const size_t VERTEX_COUNT = DAY_COUNT_P2 * 2;

size_t ComputeDayIndex(const Date& date)
{
    return Date::days_count(START_DATE, date);
}

struct BudgetStats
{
    double earned = 0.0;
    double spent = 0.0;

    BudgetStats& operator+=(const BudgetStats& other)
    {
        earned += other.earned;
        spent += other.spent;
        return *this;
    }

    BudgetStats& multiply_earned(double factor)
    {
        earned *= factor;
        return *this;
    }

    BudgetStats operator+(const BudgetStats& other)
    {
        return {earned + other.earned, spent + other.spent};
    }

    BudgetStats operator*(double factor) const
    {
        return {earned * factor, spent * factor};
    }
};

array<BudgetStats, VERTEX_COUNT> tree_values, tree_add;
array<double, VERTEX_COUNT> tree_factor;

void Init()
{
    tree_values.fill({});
    tree_add.fill({});
    tree_factor.fill(1);
}

void Push(size_t v, size_t l, size_t r)
{
    for (size_t w = v * 2; w <= v * 2 + 1; ++w)
    {
        if (w < VERTEX_COUNT)
        {
            tree_factor[w] *= tree_factor[v];
            tree_add[w].multiply_earned(tree_factor[v]) += tree_add[v];
            tree_values[w].multiply_earned(tree_factor[v]) += tree_add[v] * ((r - l) / 2.0);
        }
    }
    tree_factor[v] = 1;
    tree_add[v] = {};
}

BudgetStats ComputeSum(size_t v, size_t l, size_t r, size_t ql, size_t qr)
{
    if (v >= VERTEX_COUNT || qr <= l || r <= ql)
    {
        return {};
    }
    Push(v, l, r);
    if (ql <= l && r <= qr)
    {
        return tree_values[v];
    }
    return ComputeSum(v * 2, l, (l + r) / 2, ql, qr) + ComputeSum(v * 2 + 1, (l + r) / 2, r, ql, qr);
}

void Add(size_t v, size_t l, size_t r, size_t ql, size_t qr, double value)
{
    if (v >= VERTEX_COUNT || qr <= l || r <= ql)
    {
        return;
    }
    Push(v, l, r);
    if (ql <= l && r <= qr)
    {
        tree_add[v].earned += value;
        tree_values[v].earned += value * (r - l);
        return;
    }
    Add(v * 2, l, (l + r) / 2, ql, qr, value);
    Add(v * 2 + 1, (l + r) / 2, r, ql, qr, value);
    tree_values[v] = (v * 2 < VERTEX_COUNT ? tree_values[v * 2] : BudgetStats{}) +
                     (v * 2 + 1 < VERTEX_COUNT ? tree_values[v * 2 + 1] : BudgetStats{});
}

void Spend(size_t v, size_t l, size_t r, size_t ql, size_t qr, double value)
{
    if (v >= VERTEX_COUNT || qr <= l || r <= ql)
    {
        return;
    }
    Push(v, l, r);
    if (ql <= l && r <= qr)
    {
        tree_add[v].spent += value;
        tree_values[v].spent += value * (r - l);
        return;
    }
    Spend(v * 2, l, (l + r) / 2, ql, qr, value);
    Spend(v * 2 + 1, (l + r) / 2, r, ql, qr, value);
    tree_values[v] = (v * 2 < VERTEX_COUNT ? tree_values[v * 2] : BudgetStats{}) +
                     (v * 2 + 1 < VERTEX_COUNT ? tree_values[v * 2 + 1] : BudgetStats{});
}

void Multiply(size_t v, size_t l, size_t r, size_t ql, size_t qr, double tax_percentage)
{
    if (v >= VERTEX_COUNT || qr <= l || r <= ql)
    {
        return;
    }
    Push(v, l, r);
    if (ql <= l && r <= qr)
    {
        const double factor = 1.0 - tax_percentage / 100.0;
        tree_factor[v] *= factor;
        tree_add[v].multiply_earned(factor);
        tree_values[v].multiply_earned(factor);
        return;
    }
    Multiply(v * 2, l, (l + r) / 2, ql, qr, tax_percentage);
    Multiply(v * 2 + 1, (l + r) / 2, r, ql, qr, tax_percentage);
    tree_values[v] = (v * 2 < VERTEX_COUNT ? tree_values[v * 2] : BudgetStats{}) +
                     (v * 2 + 1 < VERTEX_COUNT ? tree_values[v * 2 + 1] : BudgetStats{});
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.precision(25);

    assert(DAY_COUNT <= DAY_COUNT_P2 && DAY_COUNT_P2 < DAY_COUNT * 2);

    Init();

    int q;
    cin >> q;

    for (int i = 0; i < q; ++i)
    {
        string query_type;
        cin >> query_type;

        string date_from_str, date_to_str;
        cin >> date_from_str >> date_to_str;

        auto idx_from = ComputeDayIndex(Date::from_string(date_from_str));
        auto idx_to = ComputeDayIndex(Date::from_string(date_to_str)) + 1;

        if (query_type == "ComputeIncome")
        {
            const auto result = ComputeSum(1, 0, DAY_COUNT_P2, idx_from, idx_to);
            cout << result.earned - result.spent << endl;
        }
        else if (query_type == "PayTax")
        {
            int value;
            cin >> value;
            Multiply(1, 0, DAY_COUNT_P2, idx_from, idx_to, value);
        }
        else if (query_type == "Earn")
        {
            double value;
            cin >> value;
            Add(1, 0, DAY_COUNT_P2, idx_from, idx_to, value / (idx_to - idx_from));
        }
        else if (query_type == "Spend")
        {
            double value;
            cin >> value;
            Spend(1, 0, DAY_COUNT_P2, idx_from, idx_to, value / (idx_to - idx_from));
        }
    }

    return 0;
}
