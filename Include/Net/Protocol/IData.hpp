#pragma once

#include "Net/Protocol/ID.hpp"
#include <vector>

namespace Net::Protocol
{
    class IData
    {
    public:
        virtual ~IData() = default;
        virtual ID getId() = 0;
        virtual std::vector<std::byte> serialize() = 0;
    };
}