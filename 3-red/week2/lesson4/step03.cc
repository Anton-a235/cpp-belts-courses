#include <cstdint>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>

class HotelManager
{
public:
    void NewBook(int64_t time, const std::string &hotel_name, size_t client_id, unsigned room_count)
    {
        books_.push({time, hotel_name, client_id, room_count});
        ++clients_[hotel_name][client_id];
        rooms_[hotel_name] += room_count;
        current_time_ = time;
        CleanHistory();
    }

    size_t Clients(const std::string &hotel_name) const
    {
        if (clients_.count(hotel_name) == 0)
        {
            return 0;
        }

        return clients_.at(hotel_name).size();
    }

    unsigned Rooms(const std::string &hotel_name) const
    {
        if (rooms_.count(hotel_name) == 0)
        {
            return 0;
        }

        return rooms_.at(hotel_name);
    }

private:
    struct Book
    {
        int64_t time;
        std::string hotel_name;
        size_t client_id;
        unsigned room_count;
    };

    static constexpr int64_t TIME_PERIOD = 86400;

    std::queue<Book> books_;
    std::unordered_map<std::string, std::unordered_map<size_t, size_t>> clients_;
    std::unordered_map<std::string, unsigned> rooms_;

    int64_t current_time_;

    void CleanHistory()
    {
        while (!books_.empty() && books_.front().time <= current_time_ - TIME_PERIOD)
        {
            const auto &book = books_.front();
            rooms_[book.hotel_name] -= book.room_count;

            auto &clients = clients_[book.hotel_name];
            --clients[book.client_id];

            if (clients[book.client_id] == 0)
            {
                clients.erase(book.client_id);
            }

            books_.pop();
        }
    }
};

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    HotelManager manager;

    size_t query_count;
    std::cin >> query_count;

    while (query_count--)
    {
        std::string query_type;
        std::cin >> query_type;

        std::string hotel_name;

        if (query_type == "BOOK")
        {
            int64_t time;
            size_t client_id;
            unsigned room_count;

            std::cin >> time >> hotel_name >> client_id >> room_count;
            manager.NewBook(time, hotel_name, client_id, room_count);
        }
        else if (query_type == "CLIENTS")
        {
            std::cin >> hotel_name;
            std::cout << manager.Clients(hotel_name) << '\n';
        }
        else if (query_type == "ROOMS")
        {
            std::cin >> hotel_name;
            std::cout << manager.Rooms(hotel_name) << '\n';
        }
    }

    return 0;
}
