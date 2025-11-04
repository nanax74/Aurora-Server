#pragma once

#include <string>
#include <cstdint>
#include <memory>

#include "Net/IConnectionSocket.hpp"

namespace Net
{
    class TcpConnectionSocket : public IConnectionSocket
    {
    private:
        friend class PollManager;

        int fd;
        std::string ip;
        uint16_t port;

        TcpConnectionSocket(int fd, std::string ip, uint16_t port);

    public:
        ~TcpConnectionSocket();
        
        static std::unique_ptr<TcpConnectionSocket> create(int fd, std::string ip, uint16_t port);

        bool send(const std::vector<std::byte>& data) override;
        std::optional<int> getReceivable() override;
        std::optional<std::vector<std::byte>> receive() override;
        int getFd() override;

        void onReadable() override;
        void onWritable() override;
        void onDisconnected() override;
    };
}