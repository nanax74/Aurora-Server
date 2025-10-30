#pragma once

#include <memory>

/*
namespace Aurora::Net::Transport
{
    class Connection
    {
    private:
        std::unique_ptr<Socket::Interface> socket;

    public:
        explicit Connection(std::unique_ptr<Socket::Interface> socket) : socket(std::move(socket)) {}

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
    };
}
*/