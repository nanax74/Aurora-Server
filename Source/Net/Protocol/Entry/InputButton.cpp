#include "Net/Protocol/Entry.hpp"
#include "Net/Transport/Connection.hpp"
#include "Data/ByteWriter.hpp"
#include "Data/ByteReader.hpp"
#include <memory>
#include "OS/Control.hpp"

namespace Net::Protocol::Entry
{
    InputButton::InputButton(uint32_t button, uint32_t timeMs) : button(button), timeMs(timeMs) {}

    ID InputButton::getId()
    {
        return ID::InputButton;
    }

    std::vector<std::byte> InputButton::serialize()
    {
        Data::ByteWriter writer;
        writer.write<uint32_t>(button);
        writer.write<uint32_t>(timeMs);
        return writer.copy();
    }

    std::unique_ptr<InputButton> InputButton::deserialize(const std::vector<std::byte>& data)
    {
        Data::ByteReader reader(data);
        uint32_t button = reader.read<uint32_t>();
        uint32_t timeMs = reader.read<uint32_t>();
        return std::unique_ptr<InputButton>(new InputButton(button, timeMs));
    }

    void InputButton::process(std::shared_ptr<Transport::Connection> connection)
    {
        OS::Control::addButton(button, timeMs);
    }
}