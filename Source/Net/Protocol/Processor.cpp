#include "Net/Protocol/Processor.hpp"
#include "Data/ByteWriter.hpp"
#include "Net/Protocol/Entry.hpp"
#include "Net/Protocol/Registry.hpp"
#include "Data/ByteReader.hpp"
#include <memory>

#include "Debug/Logger.hpp"

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

        registry->registerFactory(ID::Pong, [](std::vector<std::byte> data)
        {
            Debug::Logger::printf("ID::Pong factory: Payload size: {}", data.size());
            if (data.size() >= 4) {
                Debug::Logger::printf("ID::Pong factory: First 4 bytes of payload: {:02X} {:02X} {:02X} {:02X}",
                                      static_cast<unsigned int>(data[0]),
                                      static_cast<unsigned int>(data[1]),
                                      static_cast<unsigned int>(data[2]),
                                      static_cast<unsigned int>(data[3]));
            }
            ByteReader reader(data);
            int len = reader.read<int>();
            Debug::Logger::printf("ID::Pong factory: Read length (len): {}", len);
            Debug::Logger::printf("ID::Pong factory: Remaining data size after reading len: {}", data.size() - reader.position());
            std::string msg = reader.readString(len);
            Debug::Logger::printf("ID::Pong factory: Read string: {}", msg);
            return std::unique_ptr<Entry::Pong>(new Entry::Pong(msg));
        });
    }

    void Processor::dispatch(std::shared_ptr<Registry> registry, std::unique_ptr<Transport::Connection> connection, std::unique_ptr<IData> data)
    {
        ByteWriter writer;
        std::string str = "ABCDE";
        writer.writeString(str);
        auto ret = registry->create(ID::Pong, writer.copy());
        Debug::Logger::printf("ID:{}", static_cast<int>(ret->getId()));
        connection->send(std::move(ret));
    }
}