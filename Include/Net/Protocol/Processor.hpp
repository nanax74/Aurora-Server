#pragma once

#include "Net/Protocol/IData.hpp"
#include "Net/Protocol/Registry.hpp"
#include "Net/Transport/Connection.hpp"

#include <memory>

namespace Net::Protocol
{
    class Processor
    {
    public:
        static void registerData(std::shared_ptr<Registry> registry);
        static void dispatch(std::shared_ptr<Registry> registry, std::unique_ptr<Transport::Connection> connection, std::unique_ptr<IData> data);
    };
}