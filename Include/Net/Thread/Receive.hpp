#pragma once

#include "Util/Thread.hpp"
#include "Net/Socket/Interface/Stream.hpp"
#include "Sys/Debug/Logger.hpp"

#include <mutex>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

namespace Aurora::Net::Thread
{
    class Receive : public Util::Thread
    {
    private:
        std::vector<std::unique_ptr<Net::Socket::Interface::Stream>> &clients;
        std::mutex &clientsMutex;

    public:
        Receive(std::vector<std::unique_ptr<Net::Socket::Interface::Stream>> &clients, std::mutex &clientsMutex) : Util::Thread(10), clients(clients), clientsMutex(clientsMutex) {}

        void process() override
        {
            std::lock_guard<std::mutex> lock(clientsMutex);

            Sys::Debug::Logger::printf("Client count: {}", clients.size());
            
            for(auto &client : clients)
            {
                std::vector<uint8_t> buffer(0x1000);
                if(client->recv(buffer))
                {
                    std::string str(reinterpret_cast<char const *>(buffer.data()));
                    Sys::Debug::Logger::printf("Received: {}", str);
                }
            }
        }
    };
}