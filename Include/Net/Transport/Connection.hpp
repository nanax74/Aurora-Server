#pragma once

#include <memory>

#include "Net/Transport/IConnectionSocket.hpp"
#include "Net/Protocol/IData.hpp"

namespace Net::Transport
{
    class Connection
    {
    private:
        std::shared_ptr<IConnectionSocket> socket;

    public:
        explicit Connection(std::shared_ptr<IConnectionSocket> socket);
        void send(std::unique_ptr<Protocol::IData> data);
    };
}