#pragma once

#include "Net/Protocol/IData.hpp"
#include <string>

namespace Net::Protocol::Entry
{
    class Ping : public IData
    {
    private:
        std::string msg;
    public:
        Ping(std::string msg);
        ID getId() override;
        std::vector<std::byte> serialize() override;
    };

    class Pong : public IData
    {
    private:
        std::string msg;
    public:
        Pong(std::string msg);
        ID getId() override;
        std::vector<std::byte> serialize() override;
    };
}