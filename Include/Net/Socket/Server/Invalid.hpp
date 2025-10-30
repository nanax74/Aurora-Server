#pragma once

#include "Net/Socket/Interface/Listener.hpp"
#include "Net/Socket/Client/Invalid.hpp"

#include <optional>

namespace Aurora::Net::Socket::Server
{
    class Invalid : public Interface::Listener
    {
    public:
        Invalid() {}
        ~Invalid() {}

        bool bind() override
        {
            return false;
        }

        bool listen() override
        {
            return false;
        }

        std::unique_ptr<Interface::Stream> accept() override
        {
            return std::make_unique<Client::Invalid>();
        }

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

        std::optional<bool> isClosed() override
        {
            return std::nullopt;
        }
    };
}