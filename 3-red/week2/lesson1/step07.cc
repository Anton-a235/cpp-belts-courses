#include <set>
#include <string>
#include <vector>

using namespace std;

class Learner
{
private:
    set<string> dict;

public:
    int Learn(const vector<string> &words)
    {
        size_t old_size = dict.size();
        dict.insert(words.cbegin(), words.cend());
        return static_cast<int>(dict.size() - old_size);
    }

    vector<string> KnownWords()
    {
        return {dict.cbegin(), dict.cend()};
    }
};
