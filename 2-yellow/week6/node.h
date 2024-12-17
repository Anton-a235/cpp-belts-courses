#pragma once

#include <functional>
#include <memory>
#include <string>

#include "date.h"

enum class Comparison : unsigned
{
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual
};

enum class LogicalOperation : unsigned
{
    Or,
    And
};

class Node
{
public:
    virtual ~Node() = default;

    virtual bool Evaluate(const Date &date, const std::string &event) const = 0;
};

class EmptyNode : public Node
{
public:
    bool Evaluate(const Date &date, const std::string &event) const override;
};

class DateComparisonNode : public Node
{
public:
    DateComparisonNode(Comparison cmp, const Date &date);

    bool Evaluate(const Date &date, const std::string &event) const override;

private:
    const std::function<bool(const Date &, const Date &)> &cmp_;
    Date date_;
};

class EventComparisonNode : public Node
{
public:
    EventComparisonNode(Comparison cmp, const std::string &event);

    bool Evaluate(const Date &date, const std::string &event) const override;

private:
    const std::function<bool(const std::string &, const std::string &)> &cmp_;
    std::string event_;
};

class LogicalOperationNode : public Node
{
public:
    LogicalOperationNode(LogicalOperation op,
                         std::shared_ptr<Node> left,
                         std::shared_ptr<Node> right);

    bool Evaluate(const Date &date, const std::string &event) const override;

private:
    const std::function<bool(bool, bool)> &op_;
    std::shared_ptr<Node> left_;
    std::shared_ptr<Node> right_;
};
