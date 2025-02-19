#include <algorithm>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using namespace std;

struct Person
{
    string name;
    unsigned age, income;
    bool is_male;
};

class Database
{
public:
    void ReadPeople(istream &input)
    {
        size_t count;
        input >> count;

        people_.assign(count, {});
        count_by_age_partial_sum_.assign(200, 0);
        income_partial_sum_.clear();
        popular_name_m_ = nullopt;
        popular_name_w_ = nullopt;

        unordered_map<string_view, unsigned> m_names_count;
        unordered_map<string_view, unsigned> w_names_count;

        for (Person &p : people_)
        {
            char gender;
            input >> p.name >> p.age >> p.income >> gender;
            p.is_male = gender == 'M';

            auto &names_count = p.is_male ? m_names_count : w_names_count;
            auto &cur_count = names_count[p.name];
            ++cur_count;

            if (auto &popular_name = p.is_male ? popular_name_m_ : popular_name_w_; popular_name.has_value())
            {
                if (auto &popular_count = names_count[*popular_name]; cur_count > popular_count)
                    popular_name = p.name;
                else if (cur_count == popular_count)
                    popular_name = min(string_view(p.name), *popular_name);
            }
            else
            {
                popular_name = p.name;
            }

            ++count_by_age_partial_sum_[p.age];
            income_partial_sum_.push_back(p.income);
        }

        partial_sum(count_by_age_partial_sum_.rbegin(), count_by_age_partial_sum_.rend(),
                    count_by_age_partial_sum_.rbegin());

        sort(income_partial_sum_.begin(), income_partial_sum_.end(), greater<>{});
        partial_sum(income_partial_sum_.begin(), income_partial_sum_.end(), income_partial_sum_.begin());
    }

    size_t adult_count(unsigned adult_age) const
    {
        return count_by_age_partial_sum_[adult_age];
    }

    size_t top_income_total(unsigned top_count) const
    {
        return income_partial_sum_[min<size_t>(top_count - 1, income_partial_sum_.size() - 1)];
    }

    optional<string_view> popular_name(char gender) const
    {
        const bool is_male = gender == 'M';
        return is_male ? popular_name_m_ : popular_name_w_;
    }

private:
    vector<Person> people_;
    vector<size_t> count_by_age_partial_sum_;
    vector<size_t> income_partial_sum_;
    optional<string_view> popular_name_m_;
    optional<string_view> popular_name_w_;
};

int main()
{
    const Database db = []()
    {
        Database db;
        db.ReadPeople(cin);
        return db;
    }();

    for (string command; cin >> command;)
    {
        if (command == "AGE")
        {
            unsigned adult_age;
            cin >> adult_age;
            cout << "There are " << db.adult_count(adult_age) << " adult people for maturity age " << adult_age << '\n';
        }
        else if (command == "WEALTHY")
        {
            unsigned count;
            cin >> count;
            cout << "Top-" << count << " people have total income " << db.top_income_total(count) << '\n';
        }
        else if (command == "POPULAR_NAME")
        {
            char gender;
            cin >> gender;

            if (const auto popular_name = db.popular_name(gender); popular_name.has_value())
            {
                cout << "Most popular name among people of gender " << gender << " is " << *popular_name << '\n';
            }
            else
            {
                cout << "No people of gender " << gender << '\n';
            }
        }
    }
}
