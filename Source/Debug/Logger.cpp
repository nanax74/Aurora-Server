#include "Debug/Logger.hpp"
#include <whb/log.h>
#include <whb/log_udp.h>

namespace Debug
{
    bool Logger::init()
    {
        if (WHBLogUdpInit())
        {
            isEnabled = true;
            return true;
        }
        return false;
    }

    bool Logger::shutdown()
    {
        if (WHBLogUdpDeinit())
        {
            isEnabled = false;
            return true;
        }
        return false;
    }

    bool Logger::write(const std::string& msg)
    {
        return WHBLogPrintf("%s", msg.c_str()) != 0;
    }
}
