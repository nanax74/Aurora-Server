#pragma once

#include <vector>
#include <optional>

#if defined(_WIN32)
#include <winsock2.h>
#endif

namespace Net::Transport
{
    class IConnectionSocket
    {
    public:
        virtual ~IConnectionSocket() = default;
        virtual bool send(const std::vector<std::byte>& data) = 0;
        virtual std::optional<std::vector<std::byte>> receive() = 0;
        
        #if defined(_WIN32)
        virtual SOCKET getFd() = 0;
        #else
        virtual int getFd() = 0;
        #endif

        virtual bool onReceivable() = 0;
        virtual void onDisconnected() = 0;
    };
}