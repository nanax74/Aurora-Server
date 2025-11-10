#include "Net/Protocol/Processor.hpp"
#include "Data/ByteWriter.hpp"
#include "Net/Protocol/Entry.hpp"
#include "Net/Protocol/Registry.hpp"
#include "Data/ByteReader.hpp"
#include <memory>

namespace Net::Protocol
{
    void Processor::registerData(std::shared_ptr<Registry> registry)
    {
        registry->registerFactory(ID::Ping, [](std::vector<std::byte> data)
        {
            ByteReader reader(data);
            int len = reader.read<int>();
            std::string msg = reader.readString(len);
            return std::unique_ptr<Entry::Ping>(new Entry::Ping(msg));
        });
    }

    void Processor::dispatch(std::shared_ptr<Registry> registry, std::unique_ptr<Transport::Connection> connection, std::unique_ptr<IData> data)
    {
        ByteWriter writer;
        writer.writeString("ABC");
        auto ret = registry->create(ID::Pong, writer.data());
        connection->send(std::move(ret));
    }
}