#include "Net/PollManager.hpp"
#include "Net/IConnectionSocket.hpp"

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
            int ret = poll(pollfds.data(), pollfds.size(), POLL_TIMEOUT);

            if (ret < 0)
            {
                if (errno == EINTR) continue;
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
                sock->onDisconnected();
                socketsToUnregister.push_back(sock);
            }
            else
            {
                if (p.revents & POLLIN) sock->onReadable();
                if (p.revents & POLLOUT) sock->onWritable();
            }
        }

        for (IConnectionSocket* sock : socketsToUnregister)
        {
            unregisterSocket(sock);
        }
    }
}