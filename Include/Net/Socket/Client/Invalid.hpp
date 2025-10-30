#pragma once

#include "Net/Socket/Interface/Stream.hpp"

#include <optional>

namespace Aurora::Net::Socket::Client
{
    class Invalid : public Interface::Stream
    {
    public:
        Invalid() = default;
        ~Invalid() = default;

        bool close() override
        {
            return false;
        }

        std::optional<int> getFd() override
        {
            return std::nullopt;
        }

        std::optional<Endpoint> getLocalEndpoint() override
        {
            return std::nullopt;
        }

        virtual std::optional<Endpoint> getRemoteEndpoint()
        {
            return std::nullopt;
        }

        std::optional<bool> isClosed() override
        {
            return std::nullopt;
        }

        virtual std::optional<int> recv(std::vector<uint8_t>& buffer)
        {
            return std::nullopt;
        }

        virtual bool send(const std::vector<uint8_t>& buffer)
        {
            return false;
        }

        virtual bool connect(uint32_t ip, uint16_t port)
        {
            return false;
        }
    };
}