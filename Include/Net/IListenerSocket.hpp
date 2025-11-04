#pragma once

#include <memory>

#include "Net/IConnectionSocket.hpp"

namespace Net
{
    class IListenerSocket
    {
    public:
        virtual ~IListenerSocket() = default;
        virtual std::unique_ptr<IConnectionSocket> accept() = 0;
    };
}