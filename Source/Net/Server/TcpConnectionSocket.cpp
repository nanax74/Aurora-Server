#include <memory>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>

#include "Net/Server/TcpConnectionSocket.hpp"

namespace Server
{
    TcpConnectionSocket::TcpConnectionSocket(int fd, std::string ip, uint16_t port) : fd(fd), ip(ip), port(port) {}

    std::unique_ptr<TcpConnectionSocket> TcpConnectionSocket::create(int fd, std::string ip, uint16_t port)
    {
        return std::unique_ptr<TcpConnectionSocket>(new TcpConnectionSocket(fd, ip, port));
    }

    bool TcpConnectionSocket::send(const std::vector<std::byte>& data)
    {
        return false;
    }

    std::vector<std::byte> TcpConnectionSocket::receive()
    {
        if (!fd) return {};
    
        int available = 0;
        int res = ioctl(fd, FIONREAD, &available);
        if(res == -1) return {};

        std::vector<std::byte> buffer = std::vector<std::byte>(available);
        res = ::recv(fd, buffer.data(), buffer.size(), 0);
        if (res <= 0)
        {
            return {};
        }
        
        buffer.resize((res));
        return buffer;
    }
}