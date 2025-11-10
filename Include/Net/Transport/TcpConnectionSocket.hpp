#pragma once

#include <functional>
#include <string>
#include <cstdint>
#include <memory>
#include <vector>

#include "Net/Transport/IConnectionSocket.hpp"

namespace Net::Transport
{
    class TcpConnectionSocket : public IConnectionSocket, public std::enable_shared_from_this<TcpConnectionSocket>
    {
    private:
        friend class SocketPoller;

        int fd;
        std::string ip;
        uint16_t port;

        std::function<void(std::shared_ptr<IConnectionSocket>, std::vector<std::byte>)> onReceive;

        TcpConnectionSocket(int fd, std::string ip, uint16_t port, std::function<void(std::shared_ptr<IConnectionSocket>, std::vector<std::byte>)> onReceive);

    public:
        ~TcpConnectionSocket();
        
        static std::unique_ptr<TcpConnectionSocket> create(int fd, std::string ip, uint16_t port, std::function<void(std::shared_ptr<IConnectionSocket>, std::vector<std::byte>)> onReceive);

        bool send(const std::vector<std::byte>& data) override;
        std::optional<std::vector<std::byte>> receive() override;
        int getFd() override;

        bool onReceivable() override;
        void onDisconnected() override;
    };
}