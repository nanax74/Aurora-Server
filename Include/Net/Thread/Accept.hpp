#pragma once

#include "Net/Socket/Interface/Listener.hpp"
#include "Net/Socket/Interface/Stream.hpp"
#include "Util/Thread.hpp"
#include "Sys/Debug/Logger.hpp"

#include <mutex>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace Aurora::Net::Thread
{
    class Accept : public Util::Thread
    {
    private:
        std::unique_ptr<Net::Socket::Interface::Listener> &server;
        std::vector<std::unique_ptr<Net::Socket::Interface::Stream>> &clients;
        std::mutex &clientsMutex;

    public:
        Accept(std::unique_ptr<Net::Socket::Interface::Listener> &server, std::vector<std::unique_ptr<Net::Socket::Interface::Stream>> &clients, std::mutex &clientsMutex) : Util::Thread(10), server(server), clients(clients), clientsMutex(clientsMutex) {}

        void process() override
        {
            auto client = server->accept();

            auto fd = client->getFd();

            Socket::Endpoint endpoint = client->getRemoteEndpoint().value_or(Socket::Endpoint{0, 0});
            uint32_t ip = endpoint.ip;
            uint16_t port = endpoint.port;

            if(fd)
            {
                in_addr addr;
                addr.s_addr = ip;
                char buf[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &addr, buf, sizeof(buf));
                std::string ipStr(buf);
                Sys::Debug::Logger::printf("Connected: (Fd:{} Ip:{} Port:{})", fd.value(), ipStr, port);
            }

            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                clients.push_back(std::move(client));
            }
        }
    };
}