#pragma once

#include <string>
#include <cstdint>
#include <vector>

namespace Net::Util
{
    class Convert
    {
    public:
        static std::string ipToString(const uint32_t ip);
        static uint32_t stringToIp(const std::string& ipStr);
        static std::string toHexString(const std::vector<std::byte>& data);
    };
}