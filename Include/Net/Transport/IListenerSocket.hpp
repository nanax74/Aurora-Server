#pragma once

#include <memory>
#include <functional>

#include "Net/Transport/IConnectionSocket.hpp"

namespace Net::Transport
{
    class IListenerSocket
    {
    public:
        virtual ~IListenerSocket() = default;
        virtual std::shared_ptr<IConnectionSocket> accept(std::function<void(std::shared_ptr<IConnectionSocket>, std::vector<std::byte>)> onReceive) = 0;
    };
}