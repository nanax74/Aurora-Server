#pragma once

#include <string>
#include <cstdint>
#include <memory>

#include "Net/Server/IConnectionSocket.hpp"

namespace Server
{
    class TcpConnectionSocket : public IConnectionSocket
    {
    private:
        int fd;
        std::string ip;
        uint16_t port;

        TcpConnectionSocket(int fd, std::string ip, uint16_t port);

    public:
        ~TcpConnectionSocket();
        
        static std::unique_ptr<TcpConnectionSocket> create(int fd, std::string ip, uint16_t port);

        bool send(const std::vector<std::byte>& data) override;
        std::vector<std::byte> receive() override;
    };
}