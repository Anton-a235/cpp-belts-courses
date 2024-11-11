#include <iostream>
#include <exception>
#include <string>

using namespace std;

string AskTimeServer()
{
    throw runtime_error("some error");
}

class TimeServer
{
public:
    string GetCurrentTime()
    {
        try
        {
            last_fetched_time = AskTimeServer();
        }
        catch (const system_error &)
        {
            // return current last_fetched_time
        }

        return last_fetched_time;
    }

private:
    string last_fetched_time = "00:00:00";
};

int main()
{
    TimeServer ts;
    try
    {
        cout << ts.GetCurrentTime() << endl;
    }
    catch (exception &e)
    {
        cout << "Exception got: " << e.what() << endl;
    }
    return 0;
}
