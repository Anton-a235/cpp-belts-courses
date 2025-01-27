#include "test_runner.h"

#include <string>
#include <string_view>
#include <tuple>
#include <map>
#include <set>

using namespace std;

class Translator
{
public:
    void Add(string_view source, string_view target)
    {
        const auto [source_it, _1] = words_.emplace(source);
        const auto [target_it, _2] = words_.emplace(target);

        source_target_[*source_it] = *target_it;
        target_source_[*target_it] = *source_it;
    }

    string_view TranslateForward(string_view source) const
    {
        return source_target_.count(source) > 0
                   ? source_target_.at(source)
                   : string_view{};
    }

    string_view TranslateBackward(string_view target) const
    {
        return target_source_.count(target) > 0
                   ? target_source_.at(target)
                   : string_view{};
    }

private:
    map<string_view, string_view> source_target_;
    map<string_view, string_view> target_source_;
    set<string> words_;
};

void TestSimple()
{
    Translator translator;
    translator.Add(string("okno"), string("window"));
    translator.Add(string("stol"), string("table"));

    ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
    ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
    ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestSimple);
    return 0;
}
