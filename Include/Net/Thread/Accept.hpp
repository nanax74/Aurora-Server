#pragma once

#include "Util/Thread.hpp"
#include "Net/Socket/Interface.hpp"
#include "Sys/Debug/Logger.hpp"

#include <mutex>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace Aurora::Net::Thread
{
    class Accept : public Util::Thread
    {
    private:
        std::unique_ptr<Net::Socket::Interface> &server;
        std::vector<std::unique_ptr<Net::Socket::Interface>> &clients;
        std::mutex &clientsMutex;

    public:
        Accept(std::unique_ptr<Net::Socket::Interface> &server, std::vector<std::unique_ptr<Net::Socket::Interface>> &clients, std::mutex &clientsMutex) : Util::Thread(10), server(server), clients(clients), clientsMutex(clientsMutex) {}

        void process() override
        {
            auto client = server->accept();

            auto fd = client->getFd();
            auto ip = client->getIp();
            auto port = client->getPort();

            if(fd && ip && port)
            {
                in_addr addr;
                addr.s_addr = ip.value();
                char buf[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &addr, buf, sizeof(buf));
                std::string ipStr(buf);
                Sys::Debug::Logger::printf("Connected: (Fd:{} Ip:{} Port:{})", fd.value(), ipStr, port.value());
            }

            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                clients.push_back(std::move(client));
            }
        }
    };
}