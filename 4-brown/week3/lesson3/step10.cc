#include "test_runner.h"

#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#pragma warning(disable : 4625)
#pragma warning(disable : 4626)

#pragma warning(disable : 5026)
#pragma warning(disable : 5027)

using namespace std;

struct Email
{
    Email() = default;
    Email(const string &from, const string &to, const string &body) : from(from), to(to), body(body)
    {
    }

    string from;
    string to;
    string body;
};

class Worker
{
public:
    virtual ~Worker() = default;
    virtual void Process(unique_ptr<Email> email) = 0;
    virtual void Run()
    {
        throw logic_error("Unimplemented");
    }

private:
    unique_ptr<Worker> next_worker_;

protected:
    void PassOn(unique_ptr<Email> email) const
    {
        if (next_worker_)
            next_worker_->Process(move(email));
    }

public:
    void SetNext(unique_ptr<Worker> next)
    {
        next_worker_ = move(next);
    }
};

class Reader : public Worker
{
public:
    explicit Reader(istream &in) : in_(in)
    {
    }

    void Run() override
    {
        while (in_)
        {
            auto email = make_unique<Email>();

            if (getline(in_, email->from) && getline(in_, email->to) && getline(in_, email->body))
                Process(move(email));
        }
    }

    void Process(unique_ptr<Email> email) override
    {
        PassOn(move(email));
    }

private:
    istream &in_;
};

class Filter : public Worker
{
public:
    using Function = function<bool(const Email &)>;

    explicit Filter(Function filter) : filter_(move(filter))
    {
    }

    void Process(unique_ptr<Email> email) override
    {
        if (filter_(as_const(*email)))
            PassOn(move(email));
    }

private:
    Function filter_;
};

class Copier : public Worker
{
public:
    explicit Copier(string receiver) : receiver_(move(receiver))
    {
    }

    void Process(unique_ptr<Email> email) override
    {
        unique_ptr<Email> email_copy{};
        if (receiver_ != email->to)
            email_copy = make_unique<Email>(email->from, receiver_, email->body);

        PassOn(move(email));

        if (email_copy)
            PassOn(move(email_copy));
    }

private:
    string receiver_;
};

class Sender : public Worker
{
public:
    explicit Sender(ostream &out) : out_(out)
    {
    }

    void Process(unique_ptr<Email> email) override
    {
        out_ << email->from << '\n';
        out_ << email->to << '\n';
        out_ << email->body << '\n';

        PassOn(move(email));
    }

private:
    ostream &out_;
};

class PipelineBuilder
{
public:
    // добавляет в качестве первого обработчика Reader
    explicit PipelineBuilder(istream &in)
    {
        chain_.push_back(make_unique<Reader>(in));
    }

    // добавляет новый обработчик Filter
    PipelineBuilder &FilterBy(Filter::Function filter)
    {
        chain_.push_back(make_unique<Filter>(move(filter)));
        return *this;
    }

    // добавляет новый обработчик Copier
    PipelineBuilder &CopyTo(string recipient)
    {
        chain_.push_back(make_unique<Copier>(move(recipient)));
        return *this;
    }

    // добавляет новый обработчик Sender
    PipelineBuilder &Send(ostream &out)
    {
        chain_.push_back(make_unique<Sender>(out));
        return *this;
    }

    // возвращает готовую цепочку обработчиков
    unique_ptr<Worker> Build()
    {
        for (auto it = chain_.rbegin(); it != prev(chain_.rend()); ++it)
        {
            next(it)->get()->SetNext(move(*it));
        }

        return move(chain_.front());
    }

private:
    vector<unique_ptr<Worker>> chain_;
};

void TestSanity()
{
    string input = ("erich@example.com\n"
                    "richard@example.com\n"
                    "Hello there\n"

                    "erich@example.com\n"
                    "ralph@example.com\n"
                    "Are you sure you pressed the right button?\n"

                    "ralph@example.com\n"
                    "erich@example.com\n"
                    "I do not make mistakes of that kind\n");
    istringstream inStream(input);
    ostringstream outStream;

    PipelineBuilder builder(inStream);
    builder.FilterBy(
        [](const Email &email)
        {
            return email.from == "erich@example.com";
        });
    builder.CopyTo("richard@example.com");
    builder.Send(outStream);
    auto pipeline = builder.Build();

    pipeline->Run();

    string expectedOutput = ("erich@example.com\n"
                             "richard@example.com\n"
                             "Hello there\n"

                             "erich@example.com\n"
                             "ralph@example.com\n"
                             "Are you sure you pressed the right button?\n"

                             "erich@example.com\n"
                             "richard@example.com\n"
                             "Are you sure you pressed the right button?\n");

    ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestSanity);
    return 0;
}
