#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <string_view>
#include <vector>

using namespace std;

struct DomainGreater
{
    bool operator()(const string &d1, const string &d2) const
    {
        return lexicographical_compare(d2.crbegin(), d2.crend(), d1.crbegin(), d1.crend());
    }
};

bool IsSubdomain(const string &dom, const string &subdom)
{
    return dom.size() <= subdom.size() && equal(dom.crbegin(), dom.crend(), subdom.crbegin()) &&
           (dom.size() == subdom.size() || subdom[subdom.size() - dom.size() - 1] == '.');
}

class Firewall
{
public:
    Firewall(vector<string> banned_domains)
    {
        if (banned_domains.empty())
            return;

        sort(banned_domains.rbegin(), banned_domains.rend(), DomainGreater{});
        banned_domains_.insert(move(banned_domains.front()));

        for (auto it = next(banned_domains.begin()); it != banned_domains.end(); ++it)
        {
            if (!IsSubdomain(*banned_domains_.cbegin(), *it))
                banned_domains_.insert(banned_domains_.cbegin(), move(*it));
        }
    }

    bool CheckDomain(const string &domain) const
    {
        const auto maybe_parent_dom = banned_domains_.lower_bound(domain);
        return maybe_parent_dom == banned_domains_.cend() || !IsSubdomain(*maybe_parent_dom, domain);
    }

private:
    set<string, DomainGreater> banned_domains_;
};

vector<string> ReadDomains()
{
    size_t count;
    cin >> count;

    vector<string> domains;
    for (string line; count--;)
    {
        cin >> line;
        domains.push_back(move(line));
    }
    return domains;
}

int main()
{
    const Firewall fw = []()
    {
        vector<string> banned_domains = ReadDomains();
        Firewall result(move(banned_domains));
        return result;
    }();

    const vector<string> domains_to_check = ReadDomains();
    for (const auto &domain : domains_to_check)
    {
        cout << (fw.CheckDomain(domain) ? "Good\n" : "Bad\n");
    }

    return 0;
}
