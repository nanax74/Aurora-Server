#include <optional>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "Net/Transport/TcpListenerSocket.hpp"
#include "Net/Transport/TcpConnectionSocket.hpp"
#include "Util/IP.hpp"

namespace Net::Transport
{
    TcpListenerSocket::TcpListenerSocket(int fd) : fd(fd) {}

    TcpListenerSocket::~TcpListenerSocket()
    {
        if (fd.has_value())
        {
            close(fd.value());
        }
    }

    std::unique_ptr<TcpListenerSocket> TcpListenerSocket::create(uint16_t port)
    {
        int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(fd == -1) return nullptr;

        int opt = 1;
        int res = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        if(res < 0)
        {
            close(fd);
            return nullptr;
        }

        res = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &opt, sizeof(opt));
        if(res < 0)
        {
            close(fd);
            return nullptr;
        }

        struct sockaddr_in addr{};
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;
        res = bind(fd, (sockaddr*)&addr, sizeof(addr));
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

    std::shared_ptr<IConnectionSocket> TcpListenerSocket::accept(std::function<void(std::shared_ptr<IConnectionSocket>, std::vector<std::byte>)> onReceive)
    {
        sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);

        int clientFd = ::accept(fd.value(), (sockaddr*)&addr, &addrlen);
        if(clientFd == -1) return nullptr;

        std::string ip = Util::IP::ipToString(addr.sin_addr.s_addr);
        uint16_t port = ntohs(addr.sin_port);

        return TcpConnectionSocket::create(clientFd, ip, port, onReceive);
    }
}