#pragma once

#include <vector>

namespace Net
{
    class IConnectionSocket
    {
    public:
        virtual ~IConnectionSocket() = default;
        virtual bool send(const std::vector<std::byte>& data) = 0;
        virtual std::vector<std::byte> receive() = 0;
        virtual int getFd() = 0;

        virtual void onReadable() = 0;
        virtual void onWritable() = 0;
        virtual void onDisconnected() = 0;
    };
}