#pragma once

#include <vector>
#include <thread>
#include <unordered_map>
#include <poll.h>

namespace Net
{
    class IConnectionSocket;

    class PollManager
    {
    private:
        static constexpr int POLL_TIMEOUT = 1000 / 144; // 144Hz

        std::vector<struct pollfd> pollfds;
        std::unordered_map<int, IConnectionSocket*> socketMap;

        std::thread thread;
        std::atomic<bool> running{false};

    private:
        void processEvents();

    public:
        void registerSocket(IConnectionSocket* socket);
        void unregisterSocket(IConnectionSocket* socket);

        void run();
        void stop();
        void worker();
    };
}
