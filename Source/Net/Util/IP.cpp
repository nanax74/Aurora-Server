#include <string>
#include <cstdint>
#include <arpa/inet.h>

#include "Util/IP.hpp"

namespace Net::Util
{
    std::string IP::ipToString(const uint32_t ip)
    {
        struct in_addr addr;
        addr.s_addr = ip;
        char buf[INET_ADDRSTRLEN];
        const char* res = inet_ntop(AF_INET, &addr, buf, INET_ADDRSTRLEN);
        return res ? std::string(res) : "0.0.0.0";
    }

    uint32_t IP::stringToIp(const std::string& ipStr)
    {
        struct in_addr addr;
        if (inet_pton(AF_INET, ipStr.c_str(), &addr) == 1)
        {
            return addr.s_addr;
        }
        return 0;
    }
}