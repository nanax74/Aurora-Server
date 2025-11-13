#include "Debug/Logger.hpp"
#include "Memory/Reader.hpp"
#include "Net/Protocol/Entry.hpp"
#include "Net/Transport/Connection.hpp"
#include "Data/ByteWriter.hpp"
#include "Data/ByteReader.hpp"
#include "Util/String.hpp"
#include <memory>

namespace Net::Protocol::Entry
{
    ReadMemory::ReadMemory(uint32_t address, uint32_t size) : address(address), size(size) {}

    ID ReadMemory::getId()
    {
        return ID::ReadMemory;
    }

    std::vector<std::byte> ReadMemory::serialize()
    {
        Data::ByteWriter writer;
        writer.write<uint32_t>(address);
        writer.write<uint32_t>(size);
        return writer.copy();
    }

    std::unique_ptr<ReadMemory> ReadMemory::deserialize(const std::vector<std::byte>& data)
    {
        Data::ByteReader reader(data);
        if(reader.remain() < sizeof(uint32_t) * 2) return nullptr;
        uint32_t address = reader.read<uint32_t>();
        uint32_t size = reader.read<uint32_t>();
        return std::unique_ptr<ReadMemory>(new ReadMemory(address, size));
    }

    void ReadMemory::process(std::shared_ptr<Transport::Connection> connection)
    {
        auto data = Memory::Reader::readVector(address, size);
        if(!data)
        {
            Debug::Logger::printf("[Error] ReadMemory is failed");
            return;
        }
        std::unique_ptr<ByteArray> byteArray(new ByteArray(data));
        connection->send(std::move(byteArray));
    }
}