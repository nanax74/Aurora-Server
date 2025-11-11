#include "Net/Protocol/Entry.hpp"
#include "Net/Transport/Connection.hpp"
#include "Data/ByteWriter.hpp"
#include "Data/ByteReader.hpp"
#include "Debug/Logger.hpp"
#include <memory>

namespace Net::Protocol::Entry
{
    Ping::Ping(uint64_t timestamp) : timestamp(timestamp) {}

    ID Ping::getId()
    {
        return ID::Ping;
    }

    std::vector<std::byte> Ping::serialize()
    {
        Data::ByteWriter writer;
        writer.write<uint64_t>(timestamp);
        return writer.copy();
    }

    std::unique_ptr<Ping> Ping::deserialize(const std::vector<std::byte>& data)
    {
        Data::ByteReader reader(data);
        uint64_t timestamp = reader.read<uint64_t>();
        return std::unique_ptr<Ping>(new Ping(timestamp));
    }

    void Ping::process(std::unique_ptr<Transport::Connection> connection)
    {
        std::unique_ptr<Pong> pong(new Pong(timestamp));
        connection->send(std::move(pong));
    }

    Pong::Pong(uint64_t timestamp) : timestamp(timestamp) {}

    ID Pong::getId()
    {
        return ID::Pong;
    }

    std::vector<std::byte> Pong::serialize()
    {
        Data::ByteWriter writer;
        writer.write<uint64_t>(timestamp);
        return writer.copy();
    }

    std::unique_ptr<Pong> Pong::deserialize(const std::vector<std::byte>& data)
    {
        Data::ByteReader reader(data);
        uint64_t timestamp = reader.read<uint64_t>();
        return std::unique_ptr<Pong>(new Pong(timestamp));
    }

    void Pong::process(std::unique_ptr<Transport::Connection> connection)
    {
        Debug::Logger::printf("Pong: {}", timestamp);
    }
}