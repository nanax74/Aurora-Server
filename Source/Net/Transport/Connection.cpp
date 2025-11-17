#include "Net/Transport/Connection.hpp"
#include "Data/ByteWriter.hpp"
#include "Net/Protocol/IData.hpp"

namespace Net::Transport
{
    Connection::Connection(std::shared_ptr<IConnectionSocket> socket) : socket(socket) {}

    void Connection::send(std::unique_ptr<Protocol::IData> data)
    {
        Data::ByteWriter writer;
        writer.write<int>(static_cast<int>(data->getId()));
        auto payload = data->serialize();
        writer.writeBytes(payload.data(), payload.size());
        socket->send(writer.data());
    }

    std::shared_ptr<IConnectionSocket> Connection::getSocket()
    {
        return socket;
    }
}