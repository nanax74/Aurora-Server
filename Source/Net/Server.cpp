#include <stdexcept>
#include <chrono>

#include "Net/Server.hpp"
#include "Net/IConnectionSocket.hpp"

namespace Net
{
    Server::Server(uint16_t port)
    {
        listener = TcpListenerSocket::create(port);
        if (!listener)
        {
            throw std::runtime_error("Failed to create listener socket");
        }
    }

    Server::~Server()
    {
        stop();
    }

    void Server::start()
    {
        isRunning = true;
        pollManager.run();
        acceptThread = std::thread(&Server::acceptLoop, this);
    }

    void Server::stop()
    {
        isRunning = false;
        if (acceptThread.joinable())
        {
            acceptThread.join();
        }
        pollManager.stop();
    }

    void Server::acceptLoop()
    {
        while (isRunning)
        {
            std::unique_ptr<IConnectionSocket> connection = listener->accept();
            if (connection)
            {
                pollManager.registerSocket(connection.release());
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }
    }
}
