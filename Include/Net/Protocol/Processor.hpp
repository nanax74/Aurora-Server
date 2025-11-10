#pragma once

#include "Net/Protocol/IData.hpp"
#include "Net/Transport/Connection.hpp"

#include <memory>

namespace Net::Protocol
{
    class Processor
    {
    public:
        static void dispatch(std::unique_ptr<Transport::Connection> socket, std::unique_ptr<IData> data);
    };
}