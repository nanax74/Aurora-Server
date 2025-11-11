#include "Net/Transport/Connection.hpp"
#include "Net/Protocol/IData.hpp"

namespace Net::Transport
{
    Connection::Connection(std::shared_ptr<IConnectionSocket> socket) : socket(socket) {}

    void Connection::send(std::unique_ptr<Protocol::IData> data)
    {
        socket->send(data->serialize());
    }
}