#pragma once

#include <vector>
#include <optional>

namespace Net
{
    class IConnectionSocket
    {
    public:
        virtual ~IConnectionSocket() = default;
        virtual bool send(const std::vector<std::byte>& data) = 0;
        virtual std::optional<int> getReceivable() = 0;
        virtual std::optional<std::vector<std::byte>> receive() = 0;
        virtual int getFd() = 0;

        virtual void onReadable() = 0;
        virtual void onWritable() = 0;
        virtual void onDisconnected() = 0;
    };
}