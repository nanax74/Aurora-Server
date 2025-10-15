#pragma once

#include <whb/log.h>
#include <whb/log_udp.h>

#include <string>
#include <format>

namespace CC::Sys::Debug
{
    class Logger
    {
    public:
        static bool init()
        {
            if(WHBLogUdpInit())
            {
                isEnabled = true;
                return true;
            }
            return false;
        }
    
        static bool shutdown()
        {
            if(WHBLogUdpDeinit())
            {
                isEnabled = false;
                return true;
            }
            return false;
        }
    
        template<typename... Args>
        static bool printf(std::format_string<Args...> fmt, Args&&... args);
    
    private:
        static inline bool isEnabled = false;
    };

    template <typename... Args>
    bool Logger::printf(std::format_string<Args...> fmt, Args&&... args)
    {
        if (!isEnabled) return false;
        try
        {
            std::string formatted = std::format(fmt, std::forward<Args>(args)...);
            std::string final_msg = std::format("[Stardust] {}", formatted);
            return WHBLogPrintf("%s", final_msg.c_str()) != 0;
        }
        catch (const std::format_error& e)
        {
            WHBLogPrintf("[Stardust][format error] %s", e.what());
            return false;
        }
    }
}