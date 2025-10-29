#pragma once

#include <memory>
#include <optional>
#include <vector>

namespace Aurora::Net::Socket
{
    class Interface
    {
    protected:
        Interface() = default;

    public:
        virtual ~Interface() = default;

        virtual bool bind() = 0;
        virtual bool listen() = 0;
        virtual std::unique_ptr<Interface> accept() = 0;

        virtual bool close() = 0;
        virtual std::optional<int> recv(std::vector<uint8_t>& buffer) = 0;
        virtual bool send() = 0;

        virtual bool connect(uint32_t ip, uint16_t port) = 0;

        virtual std::optional<int> getFd() = 0;
        virtual std::optional<uint32_t> getIp() = 0;
        virtual std::optional<uint16_t> getPort() = 0;

        virtual std::optional<bool> isClosed() = 0;
    };
}