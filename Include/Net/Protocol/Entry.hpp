#pragma once

#include "Net/Protocol/IData.hpp"

namespace Net::Protocol::Entry
{
    class Ping : public IData
    {
    public:
        ID getId() override;
    };

    class Pong : public IData
    {
    public:
        ID getId() override;
    };
}