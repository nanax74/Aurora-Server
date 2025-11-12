#include "Debug/Notification.hpp"
#include "Net/Protocol/Entry.hpp"
#include "Net/Transport/Connection.hpp"
#include "Data/ByteWriter.hpp"
#include "Data/ByteReader.hpp"
#include <memory>
#include "Debug/Notification.hpp"

namespace Net::Protocol::Entry
{
    Notification::Notification(std::string msg) : msg(msg) {}

    ID Notification::getId()
    {
        return ID::Notification;
    }

    std::vector<std::byte> Notification::serialize()
    {
        Data::ByteWriter writer;
        writer.writeString(msg);
        return writer.copy();
    }

    std::unique_ptr<Notification> Notification::deserialize(const std::vector<std::byte>& data)
    {
        Data::ByteReader reader(data);
        std::string msg = reader.readString();
        return std::unique_ptr<Notification>(new Notification(msg));
    }

    void Notification::process(std::shared_ptr<Transport::Connection> connection)
    {
        Debug::Notification::info("{}", msg);
    }
}