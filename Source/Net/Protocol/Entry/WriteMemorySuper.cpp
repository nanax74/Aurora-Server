#include "Memory/Writer.hpp"
#include "Net/Protocol/Entry.hpp"
#include "Net/Transport/Connection.hpp"
#include "Data/ByteWriter.hpp"
#include "Data/ByteReader.hpp"
#include <memory>
#include "Debug/Logger.hpp"

namespace Net::Protocol::Entry
{
    WriteMemorySuper::WriteMemorySuper(uint32_t address, std::vector<std::byte> data) : address(address), data(data) {}

    ID WriteMemorySuper::getId()
    {
        return ID::WriteMemorySuper;
    }

    std::vector<std::byte> WriteMemorySuper::serialize()
    {
        Data::ByteWriter writer;
        writer.write<uint32_t>(address);
        writer.writeVector(data);
        return writer.copy();
    }

    std::unique_ptr<WriteMemorySuper> WriteMemorySuper::deserialize(const std::vector<std::byte>& data)
    {
        Data::ByteReader reader(data);
        if(reader.remain() < sizeof(uint32_t) * 2) return nullptr;
        uint32_t address = reader.read<uint32_t>();
        std::vector<std::byte> d = reader.readVector();
        return std::unique_ptr<WriteMemorySuper>(new WriteMemorySuper(address, d));
    }

    void WriteMemorySuper::process(std::shared_ptr<Transport::Connection> connection)
    {
        bool res = Memory::Writer::writeVectorSuper(address, data);
        if(!res)
        {
            Debug::Logger::printf("[Error] WriteMemorySuper is failed");
            return;
        }
        std::unique_ptr<Bool> b(new Bool(res));
        connection->send(std::move(b));
    }
}