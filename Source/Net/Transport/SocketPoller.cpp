#include "Net/Transport/SocketPoller.hpp"
#include "Debug/Logger.hpp"
#include "Net/Transport/IConnectionSocket.hpp"

#include <thread>
#include <unistd.h>
#include <algorithm>
#include <memory>
#include <vector>

namespace Net::Transport
{
    void SocketPoller::registerSocket(std::shared_ptr<IConnectionSocket> connection)
    {
        std::lock_guard<std::mutex> lock(mutex);
        struct pollfd pfd{};
        pfd.fd = connection->getFd();
        pfd.events = POLLIN;
        pollfds.push_back(pfd);
        socketMap[pfd.fd] = connection;
    }

    void SocketPoller::unregisterSocket(std::shared_ptr<IConnectionSocket> connection)
    {
        std::lock_guard<std::mutex> lock(mutex);
        int fd = connection->getFd();
        pollfds.erase(std::remove_if(pollfds.begin(), pollfds.end(),
            [fd](const struct pollfd& p) { return p.fd == fd; }), pollfds.end());
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

        std::lock_guard<std::mutex> lock(mutex);
        pollfds.clear();
        socketMap.clear();
    }

    void SocketPoller::worker()
    {
        while (running)
        {
            std::vector<std::shared_ptr<IConnectionSocket>> socketsToUnregister;
            bool shouldSleep = false;

            { // Critical section starts
                std::lock_guard<std::mutex> lock(mutex);

                if (pollfds.empty())
                {
                    shouldSleep = true;
                }
                else
                {
                    int ret = poll(pollfds.data(), pollfds.size(), POLL_TIMEOUT);
                    if (ret < 0)
                    {
                        Debug::Logger::printf("poll errno:{}", errno);
                        if (errno == EINTR) continue;
                        break;
                    }

                    if (ret > 0)
                    {
                        processEvents(socketsToUnregister);
                    }
                    // If ret == 0 (timeout), we don't need to do anything here,
                    // the loop will just continue.
                }
            } // Critical section ends

            // Unregister sockets outside the lock to avoid deadlock
            for (const auto& sock : socketsToUnregister)
            {
                unregisterSocket(sock);
                sock->onDisconnected();
            }

            if (shouldSleep)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(POLL_TIMEOUT));
            }
        }
    }

    // This function must be called from within a critical section
    void SocketPoller::processEvents(std::vector<std::shared_ptr<IConnectionSocket>>& socketsToUnregister)
    {
        // Create a copy of pollfds to iterate over, as the underlying collection
        // should not be modified while we are iterating. Although we are in a lock,
        // onReceivable can trigger logic that might try to unregister another socket.
        // A safer pattern is to iterate over a copy.
        const auto pollfdsCopy = pollfds;

        for (const auto& p : pollfdsCopy)
        {
            if (p.revents == 0) continue;

            auto it = socketMap.find(p.fd);
            if (it == socketMap.end()) continue;

            // Check if the socket is already marked for unregistering
            bool alreadyMarked = false;
            for(const auto& s : socketsToUnregister) {
                if(s->getFd() == p.fd) {
                    alreadyMarked = true;
                    break;
                }
            }
            if(alreadyMarked) continue;

            std::shared_ptr<IConnectionSocket> sock = it->second;

            if (p.revents & (POLLERR | POLLHUP | POLLNVAL))
            {
                socketsToUnregister.push_back(sock);
            }
            else if (p.revents & POLLIN)
            {
                bool res = sock->onReceivable();
                if (!res)
                {
                    socketsToUnregister.push_back(sock);
                }
            }
        }
    }
}