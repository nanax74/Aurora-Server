#include <stdexcept>
#include <chrono>

#include "Net/Transport/Server.hpp"
#include "Net/Transport/Connection.hpp"
#include "Net/Transport/IConnectionSocket.hpp"
#include "Net/Protocol/IData.hpp"

namespace Net::Transport
{
    Server::Server(uint16_t port, std::function<void(std::unique_ptr<Connection>, std::unique_ptr<Protocol::IData>)> dispatcher) : dispatcher(dispatcher)
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
        poller.run();
        acceptThread = std::thread(&Server::acceptLoop, this);
    }

    void Server::stop()
    {
        isRunning = false;
        listener.reset();
        if (acceptThread.joinable())
        {
            acceptThread.join();
        }
        poller.stop();
    }

    void Server::acceptLoop()
    {
        while (isRunning)
        {
            std::shared_ptr<IConnectionSocket> connection = listener->accept([this](std::shared_ptr<IConnectionSocket> socket, std::vector<std::byte> data){ onReceive(socket, data); });
            if (connection)
            {
                poller.registerSocket(connection);
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
            }
        }
    }

    void Server::onReceive(std::shared_ptr<IConnectionSocket> socket, std::vector<std::byte> data)
    {
        std::unique_ptr<Connection> connection(new Connection(socket));
    }
}
