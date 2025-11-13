#include "Net/Protocol/Entry.hpp"
#include "Net/Transport/Connection.hpp"
#include "Data/ByteWriter.hpp"
#include "Data/ByteReader.hpp"
#include <memory>
#include "OS/Control.hpp"

namespace Net::Protocol::Entry
{
    InputRightStick::InputRightStick(float x, float y, uint32_t timeMs) : x(x), y(y), timeMs(timeMs) {}

    ID InputRightStick::getId()
    {
        return ID::InputRightStick;
    }

    std::vector<std::byte> InputRightStick::serialize()
    {
        Data::ByteWriter writer;
        writer.write<float>(x);
        writer.write<float>(y);
        writer.write<uint32_t>(timeMs);
        return writer.copy();
    }

    std::unique_ptr<InputRightStick> InputRightStick::deserialize(const std::vector<std::byte>& data)
    {
        Data::ByteReader reader(data);
        float x = reader.read<float>();
        float y = reader.read<float>();
        uint32_t timeMs = reader.read<uint32_t>();
        return std::unique_ptr<InputRightStick>(new InputRightStick(x, y, timeMs));
    }

    void InputRightStick::process(std::shared_ptr<Transport::Connection> connection)
    {
        OS::Control::addRightStick(x, y, timeMs);
    }
}