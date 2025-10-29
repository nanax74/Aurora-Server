#pragma once

#include "Net/Socket/Interface.hpp"
#include "Net/Socket/PassiveClient.hpp"

#include <memory>
#include <optional>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

namespace Aurora::Net::Socket
{
    class Server : public Interface
    {
    private:
        std::optional<int> fd;
        uint16_t port;

        void socket()
        {
            int ret = ::socket(AF_INET, SOCK_STREAM, 0);
            if(ret == -1) { fd = std::nullopt; } else { fd = ret; }
        }

    public:
        Server(uint16_t port) : port(port) { socket(); }
        ~Server() { close(); }

        bool bind() override
        {
            if(!fd) return false;

            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = INADDR_ANY;

            if(::bind(fd.value(), (sockaddr*)&addr, sizeof(addr)) == 0) return true;
            return false;
        }

        bool listen() override
        {
            if(!fd) return false;

            if(::listen(fd.value(), 0x10) == 0) return true;
            return false;
        }

        std::unique_ptr<Interface> accept() override
        {
            if(!fd) return std::make_unique<Invalid>();

            sockaddr_in addr;
            socklen_t addrlen = sizeof(addr);

            int clientFd = ::accept(fd.value(), (sockaddr*)&addr, &addrlen);
            if(clientFd == -1) return std::make_unique<Invalid>();

            uint32_t ip = ntohl(addr.sin_addr.s_addr);
            uint16_t port = ntohs(addr.sin_port);
            return std::make_unique<PassiveClient>(clientFd, ip, port);
        }

        bool close() override
        {
            return (fd && ::close(fd.value()) != -1);
        }

        std::optional<int> recv(std::vector<uint8_t>& buffer) override
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

        std::optional<uint32_t> getIp() override
        {
            return std::nullopt;
        }

        std::optional<uint16_t> getPort() override
        {
            return port;
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