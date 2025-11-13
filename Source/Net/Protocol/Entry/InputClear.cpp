#include "Net/Protocol/Entry.hpp"
#include "Net/Transport/Connection.hpp"
#include "Data/ByteWriter.hpp"
#include "Data/ByteReader.hpp"
#include "OS/Control.hpp"
#include <memory>

namespace Net::Protocol::Entry
{
    InputClear::InputClear() {}

    ID InputClear::getId()
    {
        return ID::InputClear;
    }

    std::vector<std::byte> InputClear::serialize()
    {
        Data::ByteWriter writer;
        return writer.copy();
    }

    std::unique_ptr<InputClear> InputClear::deserialize(const std::vector<std::byte>& data)
    {
        Data::ByteReader reader(data);
        return std::unique_ptr<InputClear>(new InputClear());
    }

    void InputClear::process(std::shared_ptr<Transport::Connection> connection)
    {
        OS::Control::clear();
    }
}