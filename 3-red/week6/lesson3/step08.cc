#include "test_runner.h"
#include "profile.h"

#include <functional>
#include <future>
#include <map>
#include <set>
#include <sstream>
#include <string>

using namespace std;

struct Stats
{
    map<string, int> word_frequences;

    void operator+=(const Stats &other)
    {
        for (const auto &[k, v] : other.word_frequences)
        {
            word_frequences[k] += v;
        }
    }
};

Stats ExploreLine(const set<string> &key_words, const string &line)
{
    Stats result;
    istringstream ss(line);
    string str;

    while (ss >> str)
    {
        if (key_words.count(str) > 0)
        {
            ++result.word_frequences[move(str)];
        }
    }

    return result;
}

Stats ExploreKeyWords(const set<string> &key_words, istream &input)
{
    constexpr size_t thread_count = 4;

    vector<string> lines(thread_count);
    vector<future<Stats>> tasks;
    Stats result;

    while (input)
    {
        string line;
        for (size_t i = 0; getline(input, line); ++i)
        {
            lines[i % thread_count] += line + ' ';
        }
    }

    for (size_t i = 0; i < thread_count; ++i)
    {
        tasks.push_back(async(ExploreLine, cref(key_words), cref(lines[i])));
    }

    for (size_t i = 0; i < thread_count; ++i)
    {
        result += tasks[i].get();
    }

    return result;
}

void TestBasic()
{
    const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

    stringstream ss;
    ss << "this new yangle service really rocks\n";
    ss << "It sucks when yangle isn't available\n";
    ss << "10 reasons why yangle is the best IT company\n";
    ss << "yangle rocks others suck\n";
    ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

    const auto stats = ExploreKeyWords(key_words, ss);
    const map<string, int> expected = {
        {"yangle", 6},
        {"rocks", 2},
        {"sucks", 1}};
    ASSERT_EQUAL(stats.word_frequences, expected);
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestBasic);
}
