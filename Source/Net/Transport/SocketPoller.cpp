#include "Net/Transport/SocketPoller.hpp"
#include "Debug/Logger.hpp"
#include "Net/Transport/IConnectionSocket.hpp"
#include "poll.h"

#include <thread>
#include <unistd.h>
#include <algorithm>
#include <memory>

namespace Net::Transport
{
    void SocketPoller::registerSocket(std::shared_ptr<IConnectionSocket> connection)
    {
        struct pollfd pfd{};
        pfd.fd = connection->getFd();
        pfd.events = POLLIN;
        pollfds.push_back(pfd);
        socketMap[pfd.fd] = connection;
    }

    void SocketPoller::unregisterSocket(std::shared_ptr<IConnectionSocket> connection)
    {
        int fd = connection->getFd();
        pollfds.erase(std::remove_if(pollfds.begin(), pollfds.end(),
            [fd](auto& p) { return p.fd == fd; }), pollfds.end());
        socketMap.erase(fd);
    }

    void SocketPoller::run()
    {
        running = true;
        thread = std::thread(&SocketPoller::worker, this);
    }

    void SocketPoller::stop()
    {
        running = false;
        if (thread.joinable()) thread.join();
    }

    void SocketPoller::worker()
    {
        while (running)
        {
            if (pollfds.empty())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(POLL_TIMEOUT));
                continue;
            }
            int ret = poll(pollfds.data(), pollfds.size(), POLL_TIMEOUT);
            if (ret < 0)
            {
                Debug::Logger::printf("poll errno:{}", errno);
                if (errno == EINTR) continue;
                break;
            }

            if (ret == 0) continue;

            processEvents();
        }
    }

    void SocketPoller::processEvents()
    {
        std::vector<std::shared_ptr<IConnectionSocket>> socketsToUnregister;

        for (auto& p : pollfds)
        {
            if (p.revents == 0) continue;

            auto it = socketMap.find(p.fd);
            if (it == socketMap.end()) continue;

            std::shared_ptr<IConnectionSocket> sock = it->second;

            if (p.revents & (POLLERR | POLLHUP | POLLNVAL))
            {
                socketsToUnregister.push_back(sock);
            }
            else if(p.revents & POLLIN)
            {
                bool res = sock->onReceivable();
                if(!res)
                {
                    socketsToUnregister.push_back(sock);
                }
            }
        }

        for (std::shared_ptr<IConnectionSocket> sock : socketsToUnregister)
        {
            unregisterSocket(sock);
            sock->onDisconnected();
        }
    }
}