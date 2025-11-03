#pragma once

#include <memory>

#include "Net/Server/IConnectionSocket.hpp"

namespace Server
{
    class IListenerSocket
    {
    public:
        virtual ~IListenerSocket() = default;
        virtual std::unique_ptr<IConnectionSocket> accept() = 0;
    };
}