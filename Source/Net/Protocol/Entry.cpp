#include "Net/Protocol/Entry.hpp"
#include "Data/ByteWriter.hpp"

namespace Net::Protocol::Entry
{
    Ping::Ping(std::string msg) : msg(msg) {}

    ID Ping::getId()
    {
        return ID::Ping;
    }

    std::vector<std::byte> Ping::serialize()
    {
        return {};
    }

    Pong::Pong(std::string msg) : msg(msg) {}

    ID Pong::getId()
    {
        return ID::Pong;
    }

    std::vector<std::byte> Pong::serialize()
    {
        ByteWriter writer;

        writer.writeString(msg);
        return writer.copy();
    }
}