#pragma once

#include "Util/Thread.hpp"
#include "Net/Transport/ConnectionManager.hpp"
#include <functional>

namespace Aurora::Net::Transport
{
    class Dispatcher : public Util::Thread
    {
    private:
        ConnectionManager& manager;

    public:
        using Processor = std::function<void(Connection& connection, std::vector<uint8_t> buffer)>;
        Processor processor;

        explicit Dispatcher(ConnectionManager& mgr, Processor processor) : Util::Thread(0), manager(mgr), processor(processor) {}

        void start() override {}

        void update() override
        {
            manager.forEach([this](Connection& connection)
            {
                std::vector<uint8_t> buffer(1024);
                auto n = connection.recv(buffer);
                if (n)
                {
                    if(processor) processor(connection, buffer);
                }
            });
        }

        void end() override {}
    };
}
