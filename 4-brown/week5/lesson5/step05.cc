#pragma warning(disable : 4710)
#pragma warning(disable : 4711)

#include <cmath>
#include <cstdint>
#include <ctime>
#include <exception>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <system_error>
#include <type_traits>
#include <unordered_map>
#include <vector>

#pragma warning(disable : 4244)
#pragma warning(disable : 4365)
#pragma warning(disable : 4623)
#pragma warning(disable : 4625)
#pragma warning(disable : 4626)
#pragma warning(disable : 5027)
#pragma warning(disable : 5219)

using namespace std;

template <typename It>
class Range
{
public:
    Range(It begin, It end)
        : begin_(begin), end_(end)
    {
    }
    It begin() const
    {
        return begin_;
    }
    It end() const
    {
        return end_;
    }

private:
    It begin_;
    It end_;
};

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

struct IndexSegment
{
    size_t left;
    size_t right;

    size_t length() const
    {
        return right - left;
    }
    bool empty() const
    {
        return length() == 0;
    }

    bool Contains(IndexSegment other) const
    {
        return left <= other.left && other.right <= right;
    }
};

IndexSegment IntersectSegments(IndexSegment lhs, IndexSegment rhs)
{
    const size_t left = max(lhs.left, rhs.left);
    const size_t right = min(lhs.right, rhs.right);
    return {left, max(left, right)};
}

bool AreSegmentsIntersected(IndexSegment lhs, IndexSegment rhs)
{
    return !(lhs.right <= rhs.left || rhs.right <= lhs.left);
}

struct BudgetStats
{
    double earned = 0.0;
    double spent = 0.0;
};

BudgetStats operator+(const BudgetStats& lhs, const BudgetStats& rhs)
{
    return {lhs.earned + rhs.earned, lhs.spent + rhs.spent};
}

struct BulkMoneyAdder
{
    double delta = 0.0;
};

struct BulkMoneySpender
{
    double delta = 0.0;
};

struct BulkTaxApplier
{
    double factor = 1.0;
};

class BulkLinearUpdater
{
public:
    BulkLinearUpdater() = default;

    BulkLinearUpdater(const BulkMoneyAdder& add)
        : add_(add)
    {
    }

    BulkLinearUpdater(const BulkMoneySpender& spend)
        : spend_(spend)
    {
    }

    BulkLinearUpdater(const BulkTaxApplier& tax)
        : tax_(tax)
    {
    }

    void CombineWith(const BulkLinearUpdater& other)
    {
        tax_.factor *= other.tax_.factor;
        add_.delta = add_.delta * other.tax_.factor + other.add_.delta;
        spend_.delta += other.spend_.delta;
    }

    BudgetStats Collapse(BudgetStats origin, IndexSegment segment) const
    {
        return {origin.earned * tax_.factor + add_.delta * segment.length(),
                origin.spent + spend_.delta * segment.length()};
    }

private:
    // apply tax first, then add
    BulkTaxApplier tax_;
    BulkMoneyAdder add_;
    BulkMoneySpender spend_;
};

template <typename Data, typename BulkOperation>
class SummingSegmentTree
{
public:
    SummingSegmentTree(size_t size)
        : root_(Build({0, size}))
    {
    }

    Data ComputeSum(IndexSegment segment) const
    {
        return this->TraverseWithQuery(root_, segment, ComputeSumVisitor{});
    }

    void AddBulkOperation(IndexSegment segment, const BulkOperation& operation)
    {
        this->TraverseWithQuery(root_, segment, AddBulkOperationVisitor{operation});
    }

private:
    struct Node;
    using NodeHolder = unique_ptr<Node>;

    struct Node
    {
        NodeHolder left;
        NodeHolder right;
        IndexSegment segment;
        Data data;
        BulkOperation postponed_bulk_operation;
    };

    NodeHolder root_;

    static NodeHolder Build(IndexSegment segment)
    {
        if (segment.empty())
        {
            return nullptr;
        }
        else if (segment.length() == 1)
        {
            return make_unique<Node>(Node{
                nullptr,
                nullptr,
                segment,
            });
        }
        else
        {
            const size_t middle = segment.left + segment.length() / 2;
            return make_unique<Node>(Node{
                Build({segment.left, middle}),
                Build({middle, segment.right}),
                segment,
            });
        }
    }

    template <typename Visitor>
    static typename Visitor::ResultType TraverseWithQuery(const NodeHolder& node,
                                                          IndexSegment query_segment,
                                                          Visitor visitor)
    {
        if (!node || !AreSegmentsIntersected(node->segment, query_segment))
        {
            return visitor.ProcessEmpty(node);
        }
        else
        {
            PropagateBulkOperation(node);
            if (query_segment.Contains(node->segment))
            {
                return visitor.ProcessFull(node);
            }
            else
            {
                if constexpr (is_void_v<typename Visitor::ResultType>)
                {
                    TraverseWithQuery(node->left, query_segment, visitor);
                    TraverseWithQuery(node->right, query_segment, visitor);
                    return visitor.ProcessPartial(node, query_segment);
                }
                else
                {
                    return visitor.ProcessPartial(node,
                                                  query_segment,
                                                  TraverseWithQuery(node->left, query_segment, visitor),
                                                  TraverseWithQuery(node->right, query_segment, visitor));
                }
            }
        }
    }

    class ComputeSumVisitor
    {
    public:
        using ResultType = Data;

        Data ProcessEmpty(const NodeHolder&) const
        {
            return {};
        }

        Data ProcessFull(const NodeHolder& node) const
        {
            return node->data;
        }

        Data ProcessPartial(const NodeHolder&, IndexSegment, const Data& left_result, const Data& right_result) const
        {
            return left_result + right_result;
        }
    };

    class AddBulkOperationVisitor
    {
    public:
        using ResultType = void;

        explicit AddBulkOperationVisitor(const BulkOperation& operation)
            : operation_(operation)
        {
        }

        void ProcessEmpty(const NodeHolder&) const
        {
        }

        void ProcessFull(const NodeHolder& node) const
        {
            node->postponed_bulk_operation.CombineWith(operation_);
            node->data = operation_.Collapse(node->data, node->segment);
        }

        void ProcessPartial(const NodeHolder& node, IndexSegment) const
        {
            node->data = (node->left ? node->left->data : Data()) + (node->right ? node->right->data : Data());
        }

    private:
        const BulkOperation& operation_;
    };

    static void PropagateBulkOperation(const NodeHolder& node)
    {
        for (auto* child_ptr : {node->left.get(), node->right.get()})
        {
            if (child_ptr)
            {
                child_ptr->postponed_bulk_operation.CombineWith(node->postponed_bulk_operation);
                child_ptr->data = node->postponed_bulk_operation.Collapse(child_ptr->data, child_ptr->segment);
            }
        }
        node->postponed_bulk_operation = BulkOperation();
    }
};

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

    static Date from_string(std::string_view date)
    {
        std::tm tm{};
        std::istringstream ss({date.begin(), date.end()});
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

size_t ComputeDayIndex(const Date& date)
{
    return Date::days_count(START_DATE, date);
}

IndexSegment MakeDateSegment(const Date& date_from, const Date& date_to)
{
    return {ComputeDayIndex(date_from), ComputeDayIndex(date_to) + 1};
}

class BudgetManager : public SummingSegmentTree<BudgetStats, BulkLinearUpdater>
{
public:
    BudgetManager()
        : SummingSegmentTree(DAY_COUNT)
    {
    }
};

struct Request;
using RequestHolder = unique_ptr<Request>;

struct Request
{
    enum class Type
    {
        COMPUTE_INCOME,
        EARN,
        PAY_TAX,
        SPEND
    };

    Request(Type type)
        : type(type)
    {
    }
    static RequestHolder Create(Type type);
    virtual void ParseFrom(string_view input) = 0;
    virtual ~Request() = default;

    const Type type;
};

// clang-format off
const unordered_map<string_view, Request::Type> STR_TO_REQUEST_TYPE = {
    {"ComputeIncome", Request::Type::COMPUTE_INCOME},
    {"Earn", Request::Type::EARN},
    {"PayTax", Request::Type::PAY_TAX},
    {"Spend", Request::Type::SPEND}
};
// clang-format on

template <typename ResultType>
struct ReadRequest : Request
{
    using Request::Request;
    virtual ResultType Process(const BudgetManager& manager) const = 0;
};

struct ModifyRequest : Request
{
    using Request::Request;
    virtual void Process(BudgetManager& manager) const = 0;
};

struct ComputeIncomeRequest : ReadRequest<double>
{
    ComputeIncomeRequest()
        : ReadRequest(Type::COMPUTE_INCOME)
    {
    }
    void ParseFrom(string_view input) override
    {
        date_from = Date::from_string(ReadToken(input));
        date_to = Date::from_string(input);
    }

    double Process(const BudgetManager& manager) const override
    {
        const auto result = manager.ComputeSum(MakeDateSegment(date_from, date_to));
        return result.earned - result.spent;
    }

    Date date_from = START_DATE;
    Date date_to = START_DATE;
};

struct EarnRequest : ModifyRequest
{
    EarnRequest()
        : ModifyRequest(Type::EARN)
    {
    }
    void ParseFrom(string_view input) override
    {
        date_from = Date::from_string(ReadToken(input));
        date_to = Date::from_string(ReadToken(input));
        income = ConvertToInt(input);
    }

    void Process(BudgetManager& manager) const override
    {
        const auto date_segment = MakeDateSegment(date_from, date_to);
        const double daily_income = income * 1.0 / date_segment.length();
        manager.AddBulkOperation(date_segment, BulkMoneyAdder{daily_income});
    }

    Date date_from = START_DATE;
    Date date_to = START_DATE;
    size_t income = 0;
};

struct SpendRequest : ModifyRequest
{
    SpendRequest()
        : ModifyRequest(Type::SPEND)
    {
    }
    void ParseFrom(string_view input) override
    {
        date_from = Date::from_string(ReadToken(input));
        date_to = Date::from_string(ReadToken(input));
        outcome = ConvertToInt(input);
    }

    void Process(BudgetManager& manager) const override
    {
        const auto date_segment = MakeDateSegment(date_from, date_to);
        const double daily_outcome = outcome * 1.0 / date_segment.length();
        manager.AddBulkOperation(date_segment, BulkMoneySpender{daily_outcome});
    }

    Date date_from = START_DATE;
    Date date_to = START_DATE;
    size_t outcome = 0;
};

struct PayTaxRequest : ModifyRequest
{
    PayTaxRequest()
        : ModifyRequest(Type::PAY_TAX)
    {
    }
    void ParseFrom(string_view input) override
    {
        date_from = Date::from_string(ReadToken(input));
        date_to = Date::from_string(ReadToken(input));
        tax_percentage = ConvertToInt(input);
    }

    void Process(BudgetManager& manager) const override
    {
        manager.AddBulkOperation(MakeDateSegment(date_from, date_to), BulkTaxApplier{1.0 - tax_percentage / 100.0});
    }

    Date date_from = START_DATE;
    Date date_to = START_DATE;
    size_t tax_percentage = 0;
};

RequestHolder Request::Create(Request::Type type)
{
    switch (type)
    {
    case Request::Type::COMPUTE_INCOME:
        return make_unique<ComputeIncomeRequest>();
    case Request::Type::EARN:
        return make_unique<EarnRequest>();
    case Request::Type::PAY_TAX:
        return make_unique<PayTaxRequest>();
    case Request::Type::SPEND:
        return make_unique<SpendRequest>();
    default:
        return nullptr;
    }
}

template <typename Number>
Number ReadNumberOnLine(istream& stream)
{
    Number number;
    stream >> number;
    string dummy;
    getline(stream, dummy);
    return number;
}

optional<Request::Type> ConvertRequestTypeFromString(string_view type_str)
{
    if (const auto it = STR_TO_REQUEST_TYPE.find(type_str); it != STR_TO_REQUEST_TYPE.end())
    {
        return it->second;
    }
    else
    {
        return nullopt;
    }
}

RequestHolder ParseRequest(string_view request_str)
{
    const auto request_type = ConvertRequestTypeFromString(ReadToken(request_str));
    if (!request_type)
    {
        return nullptr;
    }
    RequestHolder request = Request::Create(*request_type);
    if (request)
    {
        request->ParseFrom(request_str);
    };
    return request;
}

vector<RequestHolder> ReadRequests(istream& in_stream = cin)
{
    const size_t request_count = ReadNumberOnLine<size_t>(in_stream);

    vector<RequestHolder> requests;
    requests.reserve(request_count);

    for (size_t i = 0; i < request_count; ++i)
    {
        string request_str;
        getline(in_stream, request_str);
        if (auto request = ParseRequest(request_str))
        {
            requests.push_back(move(request));
        }
    }
    return requests;
}

vector<double> ProcessRequests(const vector<RequestHolder>& requests)
{
    vector<double> responses;
    BudgetManager manager;
    for (const auto& request_holder : requests)
    {
        if (request_holder->type == Request::Type::COMPUTE_INCOME)
        {
            const auto& request = static_cast<const ComputeIncomeRequest&>(*request_holder);
            responses.push_back(request.Process(manager));
        }
        else
        {
            const auto& request = static_cast<const ModifyRequest&>(*request_holder);
            request.Process(manager);
        }
    }
    return responses;
}

void PrintResponses(const vector<double>& responses, ostream& stream = cout)
{
    for (const double response : responses)
    {
        stream << response << endl;
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.precision(25);

    const auto requests = ReadRequests();
    const auto responses = ProcessRequests(requests);
    PrintResponses(responses);

    return 0;
}
