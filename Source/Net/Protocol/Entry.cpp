#include "Net/Protocol/Entry.hpp"

namespace Net::Protocol::Entry
{
    ID Ping::getId()
    {
        return ID::Ping;
    }

    ID Pong::getId()
    {
        return ID::Pong;
    }
}