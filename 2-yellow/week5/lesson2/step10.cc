#include <iostream>
#include <string>

using namespace std;

// void SendSms(const string &number, const string &message)
// {
//     cout << "Send '" << message << "' to number " << number << endl;
// }

// void SendEmail(const string &email, const string &message)
// {
//     cout << "Send '" << message << "' to e-mail " << email << endl;
// }

class INotifier
{
public:
    virtual void Notify(const string &message) = 0;

    virtual ~INotifier() = default;
};

class SmsNotifier : public INotifier
{
public:
    SmsNotifier(const string &phone)
        : phone_(phone)
    {
    }

    void Notify(const string &message) override
    {
        SendSms(phone_, message);
    }

private:
    string phone_;
};

class EmailNotifier : public INotifier
{
public:
    EmailNotifier(const string &email)
        : email_(email)
    {
    }

    void Notify(const string &message) override
    {
        SendEmail(email_, message);
    }

private:
    string email_;
};

// void Notify(INotifier &notifier, const string &message)
// {
//     notifier.Notify(message);
// }

// int main()
// {
//     SmsNotifier sms{"+7-495-777-77-77"};
//     EmailNotifier email{"na-derevnyu@dedushke.ru"};

//     Notify(sms, "I have White belt in C++");
//     Notify(email, "And want a Yellow one");
//     return 0;
// }
