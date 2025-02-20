#include "Common.h"

#include <iterator>
#include <list>
#include <mutex>
#include <string>
#include <unordered_map>

using namespace std;

class LruCache : public ICache
{
public:
    LruCache(shared_ptr<IBooksUnpacker> books_unpacker, const Settings &settings)
        : books_unpacker_(move(books_unpacker)), max_memory_(settings.max_memory)
    {
    }

    BookPtr GetBook(const string &book_name) override
    {
        lock_guard lock(mtx_);

        if (const auto it = cache_.find(book_name); it != cache_.cend())
        {
            if (next(it->second) != books_.cend())
            {
                books_.splice(books_.cend(), books_, it->second);
                it->second = prev(books_.cend());
            }

            return *it->second;
        }
        else
        {
            auto book = BookPtr(books_unpacker_->UnpackBook(book_name));
            books_.push_back(book);
            cache_[book->GetName()] = prev(books_.cend());
            cache_size_ += book->GetContent().size();
            Shrink();
            return book;
        }
    }

private:
    list<BookPtr> books_;
    unordered_map<string, list<BookPtr>::const_iterator> cache_;
    size_t cache_size_{};

    shared_ptr<IBooksUnpacker> books_unpacker_;
    size_t max_memory_;
    mutex mtx_;

    void Shrink()
    {
        while (cache_size_ > max_memory_)
        {
            cache_size_ -= books_.front()->GetContent().size();
            cache_.erase(books_.front()->GetName());
            books_.erase(books_.cbegin());
        }
    }
};

unique_ptr<ICache> MakeCache(shared_ptr<IBooksUnpacker> books_unpacker, const ICache::Settings &settings)
{
    return make_unique<LruCache>(move(books_unpacker), settings);
}
