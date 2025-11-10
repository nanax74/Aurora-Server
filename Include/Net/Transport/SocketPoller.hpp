#pragma once

#include <vector>
#include <thread>
#include <unordered_map>
#include <poll.h>
#include <memory>

namespace Net::Transport
{
    class IConnectionSocket;

    class SocketPoller
    {
    private:
        static constexpr int POLL_TIMEOUT = 1000 / 144; // 144Hz

        std::vector<struct pollfd> pollfds;
        std::unordered_map<int, std::shared_ptr<IConnectionSocket>> socketMap;

        std::thread thread;
        std::atomic<bool> running{false};

    private:
        void processEvents();

    public:
        void registerSocket(std::shared_ptr<IConnectionSocket> socket);
        void unregisterSocket(std::shared_ptr<IConnectionSocket> socket);

        void run();
        void stop();
        void worker();
    };
}
