#pragma once

#include <utility>

namespace RAII
{

template <class Provider>
class Booking
{
public:
    using Id = typename Provider::BookingId;

    Booking(Provider *provider, Id id)
        : provider_(provider), id_(id)
    {
    }

    Booking(const Booking &) = delete;
    Booking &operator=(const Booking &) = delete;

    Booking(Booking &&other)
        : provider_(other.provider_), id_(other.id_)
    {
        other.provider_ = nullptr;
        other.id_ = {};
    }

    Booking &operator=(Booking &&other)
    {
        if (this != &other)
        {
            std::swap(provider_, other.provider_);
            std::swap(id_, other.id_);
        }

        return *this;
    }

    ~Booking()
    {
        if (provider_)
        {
            provider_->CancelOrComplete(*this);
        }
    }

private:
    Provider *provider_;
    Id id_;
};

} // namespace RAII
