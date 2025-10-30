#pragma once
#include "Net/Transport/Connection.hpp"

#include <vector>
#include <mutex>
#include <memory>

namespace Aurora::Net::Transport
{
    class ConnectionManager
    {
    private:
        std::vector<std::shared_ptr<Connection>> connections;
        std::mutex mutex;

    public:
        void add(std::shared_ptr<Connection> connection)
        {
            std::lock_guard lock(mutex);
            connections.push_back(connection);

            connection->onDisconnect = [this](Connection* c)
            {
                remove(c);
            };
        }

        void remove(Connection* c)
        {
            std::lock_guard lock(mutex);
            connections.erase(
            std::remove_if(
            connections.begin(),
            connections.end(),
            [c](const std::shared_ptr<Connection>& ptr){ return ptr.get() == c; }
        ),
        connections.end()
    );
        }

        template <typename Fn>
        void forEach(Fn&& fn)
        {
            std::lock_guard lock(mutex);
            for (auto& c : connections) fn(*c);
        }
    };
}
