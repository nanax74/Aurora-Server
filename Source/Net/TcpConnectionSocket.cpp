#include <memory>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

#include "Debug/Logger.hpp"
#include "Net/TcpConnectionSocket.hpp"
#include "Net/Util/Convert.hpp"

namespace Net
{
    TcpConnectionSocket::~TcpConnectionSocket() { close(fd); }

    TcpConnectionSocket::TcpConnectionSocket(int fd, std::string ip, uint16_t port) : fd(fd), ip(ip), port(port) {}

    std::unique_ptr<TcpConnectionSocket> TcpConnectionSocket::create(int fd, std::string ip, uint16_t port)
    {
        Debug::Logger::printf("Connected to: ({}:{})", ip, port);
        return std::unique_ptr<TcpConnectionSocket>(new TcpConnectionSocket(fd, ip, port));
    }

    bool TcpConnectionSocket::send(const std::vector<std::byte>& data)
    {
        return false;
    }

    std::optional<int> TcpConnectionSocket::getReceivable()
    {
        int receivable = 0;
        int res = ioctl(fd, FIONREAD, &receivable);
        if(res == -1) return std::nullopt;
        return res;
    }

    std::optional<std::vector<std::byte>> TcpConnectionSocket::receive()
    {
        if (!fd) return {};
    
        auto receivable = getReceivable();
        if(!receivable || receivable <= 0) return {};
        Debug::Logger::printf("getReceivable res:{}", receivable.value());

        std::vector<std::byte> buffer = std::vector<std::byte>(receivable.value());
        int received = ::recv(fd, buffer.data(), buffer.size(), 0);
        Debug::Logger::printf("recv res:{}", received);
        if (received <= 0) { return {}; }
        
        buffer.resize((received));
        return buffer;
    }

    int TcpConnectionSocket::getFd()
    {
        return fd;
    }

    void TcpConnectionSocket::onReadable()
    {
        auto received = receive();
        std::string hex = Util::Convert::toHexString(received.value());
        Debug::Logger::printf("Received from: ({}:{}) data: ({})", ip, port, hex);
    }

    void TcpConnectionSocket::onWritable()
    {
        
    }

    void TcpConnectionSocket::onDisconnected()
    {
        close(fd);
        Debug::Logger::printf("Disconnected from: ({}:{})", ip, port);
    }
}