#pragma once

#include "Net/Socket/Interface/Listener.hpp"
#include "Net/Socket/Server/Normal.hpp"
#include "Net/Transport/ConnectionManager.hpp"
#include "Util/Thread.hpp"

#include <memory>

namespace Aurora::Net::Transport
{
    class Acceptor : public Util::Thread
    {
    private:
        std::unique_ptr<Socket::Interface::Listener> listener;
        ConnectionManager& manager;

    public:
        Acceptor(uint16_t port, ConnectionManager& mgr) : Util::Thread(0), listener(std::make_unique<Socket::Server::Normal>(port)), manager(mgr) {}

        void start()
        {
            listener->bind();
            listener->listen();
        }

        void update()
        {
            auto stream = listener->accept();
            if (stream) manager.add(std::make_shared<Connection>(std::move(stream)));
        }

        void end() {}
    };
}
