#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>
#include <arpa/inet.h>

#include "Net/Util/Convert.hpp"

namespace Net::Util
{
    std::string Convert::ipToString(const uint32_t ip)
    {
        struct in_addr addr;
        addr.s_addr = ip;
        char buf[INET_ADDRSTRLEN];
        const char* res = inet_ntop(AF_INET, &addr, buf, INET_ADDRSTRLEN);
        return res ? std::string(res) : "0.0.0.0";
    }

    uint32_t Convert::stringToIp(const std::string& ipStr)
    {
        struct in_addr addr;
        if (inet_pton(AF_INET, ipStr.c_str(), &addr) == 1)
        {
            return addr.s_addr;
        }
        return 0;
    }

    std::string Convert::toHexString(const std::vector<std::byte>& data)
    {
        std::ostringstream oss;
        oss << std::hex << std::setfill('0');

        for (auto b : data)
            oss << std::setw(2) << static_cast<int>(std::to_integer<unsigned char>(b));

        return oss.str();
    }
}