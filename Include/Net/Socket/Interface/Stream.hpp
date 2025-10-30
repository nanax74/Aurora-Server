#pragma once

#include "Net/Socket/Endpoint.hpp"
#include "Net/Socket/Interface/Base.hpp"

#include <cstdint>
#include <vector>
#include <optional>

namespace Aurora::Net::Socket::Interface
{
    class Stream : public Base
    {
    public:
        virtual std::optional<int> recv(std::vector<uint8_t>& buffer) = 0;
        virtual bool send(const std::vector<uint8_t>& buffer) = 0;

        virtual bool connect(uint32_t ip, uint16_t port) = 0;

        virtual std::optional<Endpoint> getRemoteEndpoint() = 0;
    };
}