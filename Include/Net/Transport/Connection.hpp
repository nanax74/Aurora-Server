#pragma once

#include <memory>

#include "Net/Transport/IConnectionSocket.hpp"

namespace Net::Transport
{
    class Connection
    {
    private:
        std::shared_ptr<IConnectionSocket> socket;

    public:
        explicit Connection(std::shared_ptr<IConnectionSocket> socket);
    };
}