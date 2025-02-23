#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

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

vector<string_view> Split(string_view s, string_view delimiter = " ")
{
    vector<string_view> parts;
    if (s.empty())
    {
        return parts;
    }
    while (true)
    {
        const auto [lhs, rhs_opt] = SplitTwoStrict(s, delimiter);
        parts.push_back(lhs);
        if (!rhs_opt)
        {
            break;
        }
        s = *rhs_opt;
    }
    return parts;
}

class Domain
{
public:
    explicit Domain(string_view text)
    {
        vector<string_view> parts = Split(text, ".");
        parts_reversed_.assign(rbegin(parts), rend(parts));
    }

    size_t GetPartCount() const
    {
        return parts_reversed_.size();
    }

    auto GetParts() const
    {
        return Range(rbegin(parts_reversed_), rend(parts_reversed_));
    }
    auto GetReversedParts() const
    {
        return Range(begin(parts_reversed_), end(parts_reversed_));
    }

    // bool operator==(const Domain &other) const
    // {
    //     return parts_reversed_ == other.parts_reversed_;
    // }

private:
    vector<string> parts_reversed_;
};

// ostream &operator<<(ostream &stream, const Domain &domain)
// {
//     bool first = true;
//     for (const string_view part : domain.GetParts())
//     {
//         if (!first)
//         {
//             stream << '.';
//         }
//         else
//         {
//             first = false;
//         }
//         stream << part;
//     }
//     return stream;
// }

// domain is subdomain of itself
bool IsSubdomain(const Domain &subdomain, const Domain &domain)
{
    const auto subdomain_reversed_parts = subdomain.GetReversedParts();
    const auto domain_reversed_parts = domain.GetReversedParts();
    return subdomain.GetPartCount() >= domain.GetPartCount() &&
           equal(begin(domain_reversed_parts), end(domain_reversed_parts), begin(subdomain_reversed_parts));
}

bool IsSubOrSuperDomain(const Domain &lhs, const Domain &rhs)
{
    return lhs.GetPartCount() >= rhs.GetPartCount() ? IsSubdomain(lhs, rhs) : IsSubdomain(rhs, lhs);
}

class DomainChecker
{
public:
    template <typename InputIt>
    DomainChecker(InputIt domains_begin, InputIt domains_end)
    {
        sorted_domains_.reserve(static_cast<size_t>(distance(domains_begin, domains_end)));
        for (const Domain &domain : Range(domains_begin, domains_end))
        {
            sorted_domains_.push_back(&domain);
        }
        sort(begin(sorted_domains_), end(sorted_domains_), IsDomainLess);
        sorted_domains_ = AbsorbSubdomains(move(sorted_domains_));
    }

    // Check if candidate is subdomain of some domain
    bool IsSubdomain(const Domain &candidate) const
    {
        const auto it = upper_bound(begin(sorted_domains_), end(sorted_domains_), &candidate, IsDomainLess);
        if (it == begin(sorted_domains_))
        {
            return false;
        }
        return ::IsSubdomain(candidate, **prev(it));
    }

private:
    vector<const Domain *> sorted_domains_;

    static bool IsDomainLess(const Domain *lhs, const Domain *rhs)
    {
        const auto lhs_reversed_parts = lhs->GetReversedParts();
        const auto rhs_reversed_parts = rhs->GetReversedParts();
        return lexicographical_compare(begin(lhs_reversed_parts), end(lhs_reversed_parts), begin(rhs_reversed_parts),
                                       end(rhs_reversed_parts));
    }

    static vector<const Domain *> AbsorbSubdomains(vector<const Domain *> domains)
    {
        domains.erase(unique(begin(domains), end(domains),
                             [](const Domain *lhs, const Domain *rhs)
                             {
                                 return IsSubOrSuperDomain(*lhs, *rhs);
                             }),
                      end(domains));
        return domains;
    }
};

vector<Domain> ReadDomains(istream &in_stream = cin)
{
    vector<Domain> domains;

    size_t count;
    in_stream >> count;
    domains.reserve(count);

    for (size_t i = 0; i < count; ++i)
    {
        string domain_text;
        in_stream >> domain_text;
        domains.emplace_back(domain_text);
    }
    return domains;
}

vector<bool> CheckDomains(const vector<Domain> &banned_domains, const vector<Domain> &domains_to_check)
{
    const DomainChecker checker(begin(banned_domains), end(banned_domains));

    vector<bool> check_results;
    check_results.reserve(domains_to_check.size());
    for (const Domain &domain_to_check : domains_to_check)
    {
        check_results.push_back(!checker.IsSubdomain(domain_to_check));
    }

    return check_results;
}

void PrintCheckResults(const vector<bool> &check_results, ostream &out_stream = cout)
{
    for (const bool check_result : check_results)
    {
        out_stream << (check_result ? "Good" : "Bad") << "\n";
    }
}

void TestSplit()
{
    const auto empty = Split("", ".");
    ASSERT_EQUAL(empty, vector<string_view>());

    const auto domain_parts = Split("my.dom.ru", ".");
    const vector<string_view> expected{"my", "dom", "ru"};
    ASSERT_EQUAL(domain_parts, expected);
}

void TestGetReversedParts()
{
    const Domain domain{"my.dom.ru"};
    const vector<string> expected_parts{"my", "dom", "ru"};
    const vector<string> expected_reversed{"ru", "dom", "my"};

    const auto parts_range = domain.GetParts();
    const vector<string> parts{parts_range.begin(), parts_range.end()};
    ASSERT_EQUAL(parts, expected_parts);

    const auto parts_reversed_range = domain.GetReversedParts();
    const vector<string> parts_reversed{parts_reversed_range.begin(), parts_reversed_range.end()};
    ASSERT_EQUAL(parts_reversed, expected_reversed);
}

void TestIsSubdomain()
{
    const Domain domain{"ru"};
    const Domain subdomain1{"dom.ru"};
    const Domain subdomain2{"my.dom.ru"};

    ASSERT(IsSubdomain(domain, domain));
    ASSERT(IsSubdomain(subdomain1, subdomain1));
    ASSERT(IsSubdomain(subdomain2, subdomain2));

    ASSERT(IsSubdomain(subdomain1, domain));
    ASSERT(!IsSubdomain(domain, subdomain1));
    ASSERT(IsSubdomain(subdomain2, domain));
    ASSERT(!IsSubdomain(domain, subdomain2));
    ASSERT(IsSubdomain(subdomain2, subdomain1));
    ASSERT(!IsSubdomain(subdomain1, subdomain2));

    const Domain other{"my.dom"};

    ASSERT(!IsSubdomain(domain, other));
    ASSERT(!IsSubdomain(subdomain1, other));
    ASSERT(!IsSubdomain(subdomain2, other));
    ASSERT(!IsSubdomain(other, domain));
    ASSERT(!IsSubdomain(other, subdomain1));
    ASSERT(!IsSubdomain(other, subdomain2));
}

void TestAbsorbSubdomains()
{
    const vector<Domain> banned_domains{Domain{"dom.ru"}, Domain{"a.dom.ru"}};
    const DomainChecker checker(begin(banned_domains), end(banned_domains));
    ASSERT(checker.IsSubdomain(Domain{"b.dom.ru"}));
}

void TestCheckDomains()
{
    const vector<Domain> banned_domains{Domain{"ru"}};
    const vector<Domain> domains_to_check{Domain{"dom.ru"}, Domain{"my.dom.ru"}, Domain{"my.dom"}, Domain{"domru"}};
    const auto results = CheckDomains(banned_domains, domains_to_check);
    const vector<bool> expected{false, false, true, true};
    ASSERT_EQUAL(results, expected);
}

void TestGoodBad()
{
    const vector<Domain> banned_domains{Domain{"ru"}};
    const vector<Domain> domains_to_check{Domain{"dom.ru"}, Domain{"my.dom.ru"}, Domain{"my.dom"}, Domain{"domru"}};
    const auto results = CheckDomains(banned_domains, domains_to_check);

    ostringstream ss;
    PrintCheckResults(results, ss);
    ASSERT_EQUAL(ss.str(), "Bad\nBad\nGood\nGood\n");
}

void TestReadDomains()
{
    const string input = "3\nru\ndom.ru\nmy.dom.ru\n";
    istringstream ss(input);

    const auto domains = ReadDomains(ss);
    const vector<Domain> expected{Domain{"ru"}, Domain{"dom.ru"}, Domain{"my.dom.ru"}};
    ASSERT_EQUAL(domains, expected);
}

int main()
{
    TestRunner tr;
    RUN_TEST(tr, TestSplit);
    RUN_TEST(tr, TestGetReversedParts);
    RUN_TEST(tr, TestIsSubdomain);
    RUN_TEST(tr, TestAbsorbSubdomains);
    RUN_TEST(tr, TestCheckDomains);
    RUN_TEST(tr, TestGoodBad);
    RUN_TEST(tr, TestReadDomains);

    const vector<Domain> banned_domains = ReadDomains();
    const vector<Domain> domains_to_check = ReadDomains();
    PrintCheckResults(CheckDomains(banned_domains, domains_to_check));

    return 0;
}
