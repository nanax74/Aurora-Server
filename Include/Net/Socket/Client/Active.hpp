#pragma once

#include "Net/Socket/Interface/Stream.hpp"

#include <optional>
#include <sys/socket.h> // socket(), bind(), listen(), accept(), connect()
#include <netinet/in.h> // sockaddr_in, htons(), htonl()
#include <arpa/inet.h>  // inet_pton(), inet_ntop()
#include <unistd.h>     // close()

namespace Aurora::Net::Socket::Client
{
    class Active : public Interface::Stream
    {
    private:
        std::optional<int> fd;

        void socket()
        {
            int ret = ::socket(AF_INET, SOCK_STREAM, 0);
            if(ret != -1) { fd = ret; } else { fd = std::nullopt; }
        }

    public:
        Active() { socket(); }
        ~Active() { close(); }

        bool close() override
        {
            return (fd && ::close(fd.value()) != -1);
        }

        std::optional<int> recv(std::vector<uint8_t>& buffer)
        {
            if(fd < 0) return false;

            int ret = ::recv(fd.value(), buffer.data(), buffer.size(), 0);

            if(ret <= 0)
            {
                close();
                return std::nullopt;
            }
            return ret;
        }

        bool send(const std::vector<uint8_t>& buffer) override
        {
            if (!fd || buffer.empty()) return false;
        
            size_t totalSent = 0;
            while (totalSent < buffer.size())
            {
                ssize_t sent = ::send
                (
                    fd.value(),
                    buffer.data() + totalSent,
                    buffer.size() - totalSent,
                    0
                );

                if (sent <= 0)
                {
                    close();
                    return false;
                }
                totalSent += static_cast<size_t>(sent);
            }
            return true;
        }

        bool connect(uint32_t ip, uint16_t port) override
        {
            if(!fd) return false;

            sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = htonl(ip);

            return ::connect(fd.value(), (sockaddr*)&addr, sizeof(addr)) != -1;
        }

        std::optional<int> getFd() override
        {
            return fd;
        }

        std::optional<Endpoint> getLocalEndpoint() override
        {
            return std::nullopt; // TODO
        }
        
        std::optional<bool> isClosed() override
        {
            if (fd < 0) return true;
                
            char buf;
            int ret = ::recv(fd.value(), &buf, 1, MSG_PEEK);

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