#include "Net/PollManager.hpp"
#include "Debug/Logger.hpp"
#include "Net/IConnectionSocket.hpp"
#include "poll.h"

#include <thread>
#include <unistd.h>
#include <algorithm>

namespace Net
{
    void PollManager::registerSocket(IConnectionSocket* socket)
    {
        struct pollfd pfd{};
        pfd.fd = socket->getFd();
        pfd.events = POLLIN | POLLOUT;
        pollfds.push_back(pfd);
        socketMap[pfd.fd] = socket;
    }

    void PollManager::unregisterSocket(IConnectionSocket* socket)
    {
        int fd = socket->getFd();
        pollfds.erase(std::remove_if(pollfds.begin(), pollfds.end(),
            [fd](auto& p) { return p.fd == fd; }), pollfds.end());
        socketMap.erase(fd);
    }

    void PollManager::run()
    {
        running = true;
        thread = std::thread(&PollManager::worker, this);
    }

    void PollManager::stop()
    {
        running = false;
        if (thread.joinable()) thread.join();
    }

    void PollManager::worker()
    {
        while (running)
        {
            Debug::Logger::printf("poll() count:{}", pollfds.size());
            if (pollfds.empty())
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(POLL_TIMEOUT));
                continue;
            }
            int ret = poll(pollfds.data(), pollfds.size(), POLL_TIMEOUT);
            Debug::Logger::printf("poll ret:{}", ret);
            if (ret < 0)
            {
                Debug::Logger::printf("poll errno:{}", errno);
                if (errno == EINTR) continue;
                Debug::Logger::printf("poll break");
                break;
            }

            if (ret == 0) continue;

            processEvents();
        }
    }

    void PollManager::processEvents()
    {
        std::vector<IConnectionSocket*> socketsToUnregister;

        for (auto& p : pollfds)
        {
            if (p.revents == 0) continue;

            auto it = socketMap.find(p.fd);
            if (it == socketMap.end()) continue;

            IConnectionSocket* sock = it->second;

            if (p.revents & (POLLERR | POLLHUP | POLLNVAL))
            {
                socketsToUnregister.push_back(sock);
            }
            else
            {
                if (p.revents & POLLIN)
                {
                    auto receivable = sock->getReceivable();
                    if (!receivable || receivable <= 0)
                    {
                        socketsToUnregister.push_back(sock);
                    }
                    else
                    {
                        sock->onReadable();
                    }
                }
                if (p.revents & POLLOUT) sock->onWritable();
            }
        }

        for (IConnectionSocket* sock : socketsToUnregister)
        {
            unregisterSocket(sock);
            sock->onDisconnected();   // ←ここでclose()してOK
        }
    }
}