#pragma once

#include "Util/Thread.hpp"
#include "Net/Socket/Interface.hpp"
#include "Sys/Debug/Logger.hpp"

#include <mutex>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace Aurora::Net::Thread
{
    class Release : public Util::Thread
    {
    private:
        std::vector<std::unique_ptr<Net::Socket::Interface>> &clients;
        std::mutex &clientsMutex;

    public:
        Release(std::vector<std::unique_ptr<Net::Socket::Interface>> &clients, std::mutex &clientsMutex) : Util::Thread(10), clients(clients), clientsMutex(clientsMutex) {}

        void process() override
        {
            std::lock_guard<std::mutex> lock(clientsMutex);

            for (auto it = clients.begin(); it != clients.end();)
            {
                auto &client = *it;
            
                if (client->isClosed().value_or(true))
                {
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
                        Sys::Debug::Logger::printf("Disconnected: (Fd:{} Ip:{} Port:{})", fd.value(), ipStr, port.value());
                    }
                
                    it = clients.erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
    };
}