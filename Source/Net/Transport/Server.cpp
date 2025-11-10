#include <memory>
#include <stdexcept>
#include <chrono>

#include "Net/Transport/Server.hpp"
#include "Debug/Logger.hpp"
#include "Net/Protocol/InvalidData.hpp"
#include "Net/Transport/Connection.hpp"
#include "Net/Transport/IConnectionSocket.hpp"
#include "Net/Protocol/IData.hpp"
#include "Net/Protocol/Registry.hpp"
#include "Net/Protocol/ID.hpp"
#include "Net/Protocol/Processor.hpp"

namespace Net::Transport
{
    Server::Server(uint16_t port, std::function<void(std::shared_ptr<Protocol::Registry>, std::unique_ptr<Connection>, std::unique_ptr<Protocol::IData>)> dispatcher) : registry(std::shared_ptr<Protocol::Registry>(new Protocol::Registry())), dispatcher(dispatcher)
    {
        listener = TcpListenerSocket::create(port);
        if (!listener)
        {
            Debug::Logger::printf("Failed to create listener socket on port {}", port);
            throw std::runtime_error("Failed to create listener socket");
        }
        Debug::Logger::printf("Listener socket created on port {}", port);
        Protocol::Processor::registerData(registry);
    }

    Server::~Server()
    {
        stop();
    }

    void Server::start()
    {
        Debug::Logger::printf("Server starting...");
        isRunning = true;
        poller.run();
        acceptThread = std::thread(&Server::acceptLoop, this);
        Debug::Logger::printf("Server started.");
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
            Debug::Logger::printf("Waiting for new connection...");
            std::shared_ptr<IConnectionSocket> connection = listener->accept([this](std::shared_ptr<IConnectionSocket> socket, std::vector<std::byte> data){ onReceive(socket, data); });
            if (connection)
            {
                Debug::Logger::printf("New connection accepted. Registering with poller.");
                poller.registerSocket(connection);
                Debug::Logger::printf("Connection registered with poller.");
            }
            else
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
            }
        }
    }

    void Server::onReceive(std::shared_ptr<IConnectionSocket> socket, std::vector<std::byte> data)
    {
        Debug::Logger::printf("Received data from socket. Data size: {}", data.size());
        if (!dispatcher) return;

        auto connection = std::make_unique<Connection>(socket);

        if (data.size() < sizeof(Protocol::ID))
        {
            Debug::Logger::printf("Received data size ({}) is less than Protocol::ID size ({}). Dispatching InvalidData.", data.size(), sizeof(Protocol::ID));
            dispatcher(registry, std::move(connection), std::make_unique<Protocol::InvalidData>());
            return;
        }

        Protocol::ID id = *reinterpret_cast<const Protocol::ID*>(data.data());
        Debug::Logger::printf("Extracted Protocol::ID: {}", static_cast<uint32_t>(id));

        std::vector<std::byte> payload(data.begin() + sizeof(Protocol::ID), data.end());
        Debug::Logger::printf("Creating IData from payload with size: {}", payload.size());
        std::unique_ptr<Protocol::IData> protocolData(registry->create(id, payload));
        Debug::Logger::printf("Dispatching data for Protocol::ID: {}", static_cast<uint32_t>(id));
        dispatcher(registry, std::move(connection), std::move(protocolData));
    }
}
