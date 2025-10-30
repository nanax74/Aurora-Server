#pragma once

#include "Net/Socket/Endpoint.hpp"
#include "Net/Socket/Interface/Stream.hpp"

#include <optional>
#include <sys/socket.h> // socket(), bind(), listen(), accept(), connect()
#include <netinet/in.h> // sockaddr_in, htons(), htonl()
#include <arpa/inet.h>  // inet_pton(), inet_ntop()
#include <unistd.h>     // close()

namespace Aurora::Net::Socket::Client
{
    class Passive : public Interface::Stream
    {
    private:
        int fd;
        uint32_t ip;
        uint16_t port;

    public:
        Passive(int fd, uint32_t ip, uint16_t port) : fd(fd), ip(ip), port(port) {} // Passive
        ~Passive() { close(); }

        bool close() override
        {
            return (fd && ::close(fd) != -1);
        }

        std::optional<int> recv(std::vector<uint8_t>& buffer) override
        {
            if(fd < 0) return false;

            int ret = ::recv(fd, buffer.data(), buffer.size(), 0);

            if(ret <= 0)
            {
                close();
                return std::nullopt;
            }
            return ret;
        }

        bool send() override
        {
            return false; // TODO
        }

        bool connect(uint32_t ip, uint16_t port) override
        {
            return false;
        }

        std::optional<int> getFd() override
        {
            return fd;
        }

        std::optional<Endpoint> getLocalEndpoint() override
        {
            return std::nullopt;
        }

        std::optional<Endpoint> getRemoteEndpoint() override
        {
            return Endpoint{ip, port};
        }

        std::optional<bool> isClosed() override
        {
            if (fd < 0) return true;
                
            char buf;
            int ret = ::recv(fd, &buf, 1, MSG_PEEK);

            if (ret == 0)
            {
                close();
                return true;
            }
            else if (ret < 0)
            {
                return true;
            }
        
            return false;
        }
    };
}