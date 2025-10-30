#pragma once

#include <memory>
#include <vector>

#include "Net/Socket/Interface/Stream.hpp"
#include "Net/Socket/Interface/Listener.hpp"
#include "Net/Socket/Server/Normal.hpp"
#include "Net/Socket/Server/Invalid.hpp"
#include "Net/Thread/Accept.hpp"
#include "Net/Thread/Receive.hpp"
#include "Net/Thread/Release.hpp"
#include "Sys/Debug/Logger.hpp"
#include "Util/Singleton.hpp"

namespace Aurora
{
    class Core : public Util::Singleton<Core>
    {
    private:
        std::unique_ptr<Net::Socket::Interface::Listener> server;
        std::vector<std::unique_ptr<Net::Socket::Interface::Stream>> clients;
        std::mutex clientsMutex;

        std::unique_ptr<Util::Thread> acceptThread;
        std::unique_ptr<Util::Thread> releaseThread;
        std::unique_ptr<Util::Thread> receiveThread;

        void start()
        {
            server = std::make_unique<Net::Socket::Server::Normal>(55555);
            if(!server->bind() || !server->listen())
            {
                server = std::make_unique<Net::Socket::Server::Invalid>();
                Sys::Debug::Logger::printf("Failed to start server");
            }

            acceptThread = std::make_unique<Net::Thread::Accept>(server, clients, clientsMutex);
            acceptThread->run();

            releaseThread = std::make_unique<Net::Thread::Release>(clients, clientsMutex);
            releaseThread->run();

            receiveThread = std::make_unique<Net::Thread::Receive>(clients, clientsMutex);
            receiveThread->run();
        }

        void end()
        {
            receiveThread->stop();
            receiveThread.reset();

            releaseThread->stop();
            releaseThread.reset();

            acceptThread->stop();
            acceptThread.reset();

            server.reset();
        }

    public:
        static void run()
        {
            instance().start();
        }

        static void stop()
        {
            instance().end();
        }
    };
}