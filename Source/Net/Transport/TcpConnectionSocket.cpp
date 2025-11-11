#include <memory>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

#include "Debug/Logger.hpp"
#include "Net/Transport/TcpConnectionSocket.hpp"
#include "Net/Transport/IConnectionSocket.hpp"

namespace Net::Transport
{
    TcpConnectionSocket::~TcpConnectionSocket() { close(fd); }

    TcpConnectionSocket::TcpConnectionSocket(int fd, std::string ip, uint16_t port, std::function<void(std::shared_ptr<IConnectionSocket>, std::vector<std::byte>)> onReceive) : fd(fd), ip(ip), port(port), onReceive(onReceive) {}

    std::unique_ptr<TcpConnectionSocket> TcpConnectionSocket::create(int fd, std::string ip, uint16_t port, std::function<void(std::shared_ptr<IConnectionSocket>, std::vector<std::byte>)> onReceive)
    {
        Debug::Logger::printf("Connected to: ({}:{})", ip, port);
        return std::unique_ptr<TcpConnectionSocket>(new TcpConnectionSocket(fd, ip, port, onReceive));
    }

bool TcpConnectionSocket::send(const std::vector<std::byte>& data)
{
    int bytesSent = ::send(fd, reinterpret_cast<const char*>(data.data()), data.size(), 0);

    if (bytesSent > 0)
    {
        std::string hexStr;
        for (size_t i = 0; i < data.size(); ++i)
        {
            char buf[4];
            std::snprintf(buf, sizeof(buf), "%02X ", static_cast<unsigned int>(data[i]));
            hexStr += buf;
        }

        Debug::Logger::printf("Sent {} bytes: {}", bytesSent, hexStr.c_str());
        return true;
    }
    else
    {
        Debug::Logger::printf("Failed to send data, returned {}", bytesSent);
        return false;
    }
}

    std::optional<std::vector<std::byte>> TcpConnectionSocket::receive()
    {
        if (!fd) return std::nullopt;

        std::vector<std::byte> buffer(1024 * 4);
        int received = ::recv(fd, buffer.data(), buffer.size(), 0);

        if (received <= 0)
        {
            return std::nullopt;
        }

        buffer.resize(received);
        return buffer;
    }

    int TcpConnectionSocket::getFd()
    {
        return fd;
    }

    bool TcpConnectionSocket::onReceivable()
    {
        auto received = receive();
        if(!received) return false;
        if(onReceive) onReceive(shared_from_this(), received.value());
        return true;
    }

    void TcpConnectionSocket::onDisconnected()
    {
        Debug::Logger::printf("Disconnected from: ({}:{})", ip, port);
    }
}