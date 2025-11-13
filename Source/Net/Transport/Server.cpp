#include <memory>
#include <stdexcept>
#include <chrono>

#include "Net/Transport/Server.hpp"
#include "Debug/Logger.hpp"
#include "Net/Transport/Connection.hpp"
#include "Net/Transport/IConnectionSocket.hpp"
#include "Net/Protocol/IData.hpp"
#include "Net/Protocol/Registry.hpp"
#include "Net/Protocol/ID.hpp"
#include "Net/Protocol/Processor.hpp"
#include "Data/ByteReader.hpp"

namespace Net::Transport
{
    Server::Server(uint16_t port) : registry(std::shared_ptr<Protocol::Registry>(new Protocol::Registry()))
    {
        listener = TcpListenerSocket::create(port);
        if (!listener)
        {
            Debug::Logger::printf("Failed to create listener socket on port {}", port);
            throw std::runtime_error("Failed to create listener socket");
        }
        Protocol::Processor::registerData(registry);
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
        Debug::Logger::printf("Server started");
    }

    void Server::stop()
    {
        isRunning = false;
        listener.reset();
        poller.stop();
        if (acceptThread.joinable())
        {
            acceptThread.join();
        }
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
        auto connection = std::make_unique<Connection>(socket);

        if (data.size() < sizeof(Protocol::ID))
        {
            Debug::Logger::printf("Received data size ({}) is less than Protocol::ID size ({}). Dispatching InvalidData.", data.size(), sizeof(Protocol::ID));
            return;
        }

        Data::ByteReader reader(data);
        Protocol::ID id = static_cast<Protocol::ID>(reader.read<int>());
        Debug::Logger::printf("[Received] ID: {}", static_cast<uint32_t>(id));

        std::vector<std::byte> payload(reader.readBytes(reader.remain()));
        std::unique_ptr<Protocol::IData> protocolData(registry->deserialize(id, payload));
        if(protocolData == nullptr)
        {
            //Debug::Logger::printf("nullptr");
            return;
        }
        protocolData->process(std::move(connection));
    }
}
