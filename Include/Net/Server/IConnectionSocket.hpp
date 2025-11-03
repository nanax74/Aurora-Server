#pragma once

#include <vector>

namespace Server
{
    class IConnectionSocket
    {
    public:
        virtual ~IConnectionSocket() = default;
        virtual bool send(const std::vector<std::byte>& data);
        virtual std::vector<std::byte> receive();
    };
}