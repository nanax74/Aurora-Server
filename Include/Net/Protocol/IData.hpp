#pragma once

#include "Net/Protocol/ID.hpp"

namespace Net::Protocol
{
    class IData
    {
    public:
        virtual ~IData() = default;
        virtual ID getId() = 0;
    };
}