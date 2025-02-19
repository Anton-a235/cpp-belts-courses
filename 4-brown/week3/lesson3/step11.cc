#include "Common.h"
#include "test_runner.h"

#include <functional>
#include <sstream>

#pragma warning(disable : 4625)
#pragma warning(disable : 4626)

using namespace std;

class ValueExpression : public Expression
{
public:
    explicit ValueExpression(int val) : val_(val)
    {
    }

    int Evaluate() const override
    {
        return val_;
    }

    std::string ToString() const override
    {
        return to_string(val_);
    }

private:
    int val_;
};

class BinaryExpression : public Expression
{
public:
    using Operation = std::function<int(int, int)>;

    template <typename BinaryOp>
    explicit BinaryExpression(ExpressionPtr left, BinaryOp op, ExpressionPtr right)
        : left_(move(left)), right_(move(right))
    {
        if constexpr (is_same_v<BinaryOp, std::plus<int>>)
            op_ = {move(op), '+'};
        else if constexpr (is_same_v<BinaryOp, std::multiplies<int>>)
            op_ = {move(op), '*'};
        else
            static_assert("Operation not supported");
    }

    int Evaluate() const override
    {
        return op_.first(left_->Evaluate(), right_->Evaluate());
    }

    std::string ToString() const override
    {
        return '(' + left_->ToString() + ')' + op_.second + '(' + right_->ToString() + ')';
    }

private:
    ExpressionPtr left_;
    pair<Operation, char> op_;
    ExpressionPtr right_;
};

ExpressionPtr Value(int value)
{
    return make_unique<ValueExpression>(value);
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right)
{
    return make_unique<BinaryExpression>(move(left), std::plus<int>{}, move(right));
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right)
{
    return make_unique<BinaryExpression>(move(left), std::multiplies<int>{}, move(right));
}

string Print(const Expression *e)
{
    if (!e)
    {
        return "Null expression provided";
    }
    stringstream output;
    output << e->ToString() << " = " << e->Evaluate();
    return output.str();
}

void Test()
{
    ExpressionPtr e1 = Product(Value(2), Sum(Value(3), Value(4)));
    ASSERT_EQUAL(Print(e1.get()), "(2)*((3)+(4)) = 14");

    ExpressionPtr e2 = Sum(move(e1), Value(5));
    ASSERT_EQUAL(Print(e2.get()), "((2)*((3)+(4)))+(5) = 19");

    ASSERT_EQUAL(Print(e1.get()), "Null expression provided");
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, Test);
    return 0;
}
