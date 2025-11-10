#include "Net/Protocol/InvalidData.hpp"

namespace Net::Protocol
{
    ID InvalidData::getId()
    {
        return ID::Invalid;
    }
}