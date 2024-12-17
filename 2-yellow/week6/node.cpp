#include "node.h"

#include <exception>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

template <typename T>
static const auto &GetComparisonFunc(Comparison cmp)
{
    static const std::unordered_map<Comparison, std::function<bool(const T &, const T &)>> m{
        {Comparison::Less, std::less<T>{}},
        {Comparison::LessOrEqual, std::less_equal<T>{}},
        {Comparison::Greater, std::greater<T>{}},
        {Comparison::GreaterOrEqual, std::greater_equal<T>{}},
        {Comparison::Equal, std::equal_to<T>{}},
        {Comparison::NotEqual, std::not_equal_to<T>{}},
    };

    const auto it = m.find(cmp);

    if (it == m.cend())
    {
        throw std::logic_error(
            "Unknown comparison operation enum value: " + std::to_string(static_cast<unsigned>(cmp)));
    }

    return m.at(cmp);
}

static const auto &GetLogicalFunc(LogicalOperation op)
{
    static const std::unordered_map<LogicalOperation, std::function<bool(bool, bool)>> m{
        {LogicalOperation::Or, std::logical_or<bool>{}},
        {LogicalOperation::And, std::logical_and<bool>{}},
    };

    const auto it = m.find(op);

    if (it == m.cend())
    {
        throw std::logic_error(
            "Unknown logical operation enum value: " + std::to_string(static_cast<unsigned>(op)));
    }

    return m.at(op);
}

bool EmptyNode::Evaluate(const Date &, const std::string &) const
{
    return true;
}

DateComparisonNode::DateComparisonNode(Comparison cmp, const Date &date)
    : cmp_(GetComparisonFunc<Date>(cmp)), date_(date)
{
}

bool DateComparisonNode::Evaluate(const Date &date, const std::string &) const
{
    return cmp_(date, date_);
}

EventComparisonNode::EventComparisonNode(Comparison cmp, const std::string &event)
    : cmp_(GetComparisonFunc<std::string>(cmp)), event_(event)
{
}

bool EventComparisonNode::Evaluate(const Date &, const std::string &event) const
{
    return cmp_(event, event_);
}

LogicalOperationNode::LogicalOperationNode(LogicalOperation op,
                                           std::shared_ptr<Node> left,
                                           std::shared_ptr<Node> right)
    : op_(GetLogicalFunc(op)), left_(left), right_(right)
{
}

bool LogicalOperationNode::Evaluate(const Date &date, const std::string &event) const
{
    return op_(left_->Evaluate(date, event), right_->Evaluate(date, event));
}
