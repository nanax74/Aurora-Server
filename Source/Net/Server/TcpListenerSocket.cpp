#include <optional>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "Net/Server/TcpListenerSocket.hpp"
#include "Net/Server/TcpConnectionSocket.hpp"
#include "Net/Convert.hpp"

namespace Server
{
    TcpListenerSocket::TcpListenerSocket(int fd) : fd(fd) {}

    std::unique_ptr<TcpListenerSocket> TcpListenerSocket::create(uint16_t port)
    {
        int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(fd == -1) return nullptr;

        struct sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;
        int res = bind(fd, (sockaddr*)&addr, sizeof(addr));
        if(res == -1)
        {
            close(fd);
            return nullptr;
        }

        res = listen(fd, 16);
        if(res == -1)
        {
            close(fd);
            return nullptr;
        }

        return std::unique_ptr<TcpListenerSocket>(new TcpListenerSocket(fd));
    }

    std::unique_ptr<IConnectionSocket> TcpListenerSocket::accept()
    {
        sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);

        int clientFd = ::accept(fd.value(), (sockaddr*)&addr, &addrlen);
        if(clientFd == -1) return nullptr;

        std::string ip = Convert::ipToString(addr.sin_addr.s_addr);
        uint16_t port = ntohs(addr.sin_port);
        
        return TcpConnectionSocket::create(clientFd, ip, port);
    }
}