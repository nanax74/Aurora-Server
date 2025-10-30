#pragma once

#include "Net/Socket/Endpoint.hpp"
#include <optional>

namespace Aurora::Net::Socket::Interface
{
    class Base
    {
    protected:
        Base() = default;

    public:
        virtual ~Base() = default;

        virtual std::optional<int> getFd() = 0;
        virtual std::optional<Endpoint> getLocalEndpoint() = 0;
        virtual bool close() = 0;
        virtual std::optional<bool> isClosed() = 0;
    };
}