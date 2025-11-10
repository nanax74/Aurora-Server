#include "Net/Transport/Connection.hpp"

namespace Net::Transport
{
    Connection::Connection(std::shared_ptr<IConnectionSocket> socket) : socket(socket) {}
}