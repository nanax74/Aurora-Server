#pragma once

#include <memory>
#include <vector>

#include "Net/Socket/Interface.hpp"
#include "Net/Socket/Invalid.hpp"
#include "Net/Socket/Server.hpp"
#include "Net/Thread/Accept.hpp"
#include "Net/Thread/Release.hpp"
#include "Sys/Debug/Logger.hpp"
#include "Util/Singleton.hpp"

namespace Aurora
{
    class Core : public Util::Singleton<Core>
    {
    private:
        std::unique_ptr<Net::Socket::Interface> server;
        std::vector<std::unique_ptr<Net::Socket::Interface>> clients;
        std::mutex clientsMutex;

        std::unique_ptr<Util::Thread> acceptThread;
        std::unique_ptr<Util::Thread> releaseThread;

        void start()
        {
            server = std::make_unique<Net::Socket::Server>(55555);
            if(!server->bind() || !server->listen())
            {
                server = std::make_unique<Net::Socket::Invalid>();
                Sys::Debug::Logger::printf("Failed to start server");
            }

            acceptThread = std::make_unique<Net::Thread::Accept>(server, clients, clientsMutex);
            acceptThread->run();

            releaseThread = std::make_unique<Net::Thread::Release>(clients, clientsMutex);
            releaseThread->run();
        }

        void end()
        {
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