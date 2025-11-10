#pragma once

#include <vector>
#include <optional>

namespace Net::Transport
{
    class IConnectionSocket
    {
    public:
        virtual ~IConnectionSocket() = default;
        virtual bool send(const std::vector<std::byte>& data) = 0;
        virtual std::optional<std::vector<std::byte>> receive() = 0;
        virtual int getFd() = 0;

        virtual bool onReceivable() = 0;
        virtual void onDisconnected() = 0;
    };
}