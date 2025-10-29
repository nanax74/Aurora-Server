#pragma once

#include "Net/Socket/Interface.hpp"

#include <memory>       // std::unique_ptr, std::make_unique
#include <optional>

namespace Aurora::Net::Socket
{
    class Invalid : public Interface
    {
    public:
        Invalid() = default;
        ~Invalid() = default;

        bool bind() override
        {
            return false;
        }

        bool listen() override
        {
            return false;
        }

        std::unique_ptr<Interface> accept() override
        {
            return std::make_unique<Invalid>();
        }

        bool close() override
        {
            return false;
        }

        std::optional<int> recv(std::vector<uint8_t>& buffer) override
        {
            return std::nullopt;
        }

        bool send() override
        {
            return false;
        }

        bool connect(uint32_t ip, uint16_t port) override
        {
            return false;
        }

        std::optional<int> getFd() override
        {
            return std::nullopt;
        }

        std::optional<uint32_t> getIp() override
        {
            return std::nullopt;
        }

        std::optional<uint16_t> getPort() override
        {
            return std::nullopt;
        }

        std::optional<bool> isClosed() override
        {
            return std::nullopt;
        }
    };
}