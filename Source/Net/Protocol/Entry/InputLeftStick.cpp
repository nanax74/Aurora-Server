#include "Net/Protocol/Entry.hpp"
#include "Net/Transport/Connection.hpp"
#include "Data/ByteWriter.hpp"
#include "Data/ByteReader.hpp"
#include <memory>
#include "OS/Control.hpp"

namespace Net::Protocol::Entry
{
    InputLeftStick::InputLeftStick(float x, float y, uint32_t timeMs) : x(x), y(y), timeMs(timeMs) {}

    ID InputLeftStick::getId()
    {
        return ID::InputLeftStick;
    }

    std::vector<std::byte> InputLeftStick::serialize()
    {
        Data::ByteWriter writer;
        writer.write<float>(x);
        writer.write<float>(y);
        writer.write<uint32_t>(timeMs);
        return writer.copy();
    }

    std::unique_ptr<InputLeftStick> InputLeftStick::deserialize(const std::vector<std::byte>& data)
    {
        Data::ByteReader reader(data);
        float x = reader.read<float>();
        float y = reader.read<float>();
        uint32_t timeMs = reader.read<uint32_t>();
        return std::unique_ptr<InputLeftStick>(new InputLeftStick(x, y, timeMs));
    }

    void InputLeftStick::process(std::shared_ptr<Transport::Connection> connection)
    {
        OS::Control::addLeftStick(x, y, timeMs);
    }
}