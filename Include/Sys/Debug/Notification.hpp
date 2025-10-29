#pragma once

#include <notifications/notifications.h>

#include <coreinit/debug.h>

#include <string>
#include <format>
#include <utility>

namespace Aurora::Sys::Debug
{
    class Notification
    {
    public:
        static bool init()
        {
            if(NotificationModule_InitLibrary() == NOTIFICATION_MODULE_RESULT_SUCCESS)
            {
                NotificationModule_SetDefaultValue(NOTIFICATION_MODULE_NOTIFICATION_TYPE_INFO, NOTIFICATION_MODULE_DEFAULT_OPTION_DURATION_BEFORE_FADE_OUT, 5.0f);
                isEnabled = true;
                return true;
            }
            return false;
        }
    
        static bool shutdown()
        {
            if(NotificationModule_DeInitLibrary() == NOTIFICATION_MODULE_RESULT_SUCCESS)
            {
                isEnabled = false;
                return true;
            }
            return false;
        }
    
        template<typename... Args>
        static bool info(std::format_string<Args...> fmt, Args&&... args);
    
        template<typename... Args>
        static bool error(std::format_string<Args...> fmt, Args&&... args);
    
    private:
        static inline bool isEnabled = false;
    };

    template <typename... Args>
    bool Notification::info(std::format_string<Args...> fmt, Args&&... args)
    {
        if (!isEnabled) return false;
        try
        {
            std::string formatted = std::format(fmt, std::forward<Args>(args)...);
            std::string final_msg = std::format("[Aurora] {}", formatted);
            return NotificationModule_AddInfoNotification(final_msg.c_str()) != NOTIFICATION_MODULE_RESULT_SUCCESS;
        }
        catch (const std::format_error& e)
        {
            std::string formatted = std::format("[Aurora][format error] {}", e.what());
            NotificationModule_AddInfoNotification(formatted.c_str());
            return false;
        }
    }

    template <typename... Args>
    bool Notification::error(std::format_string<Args...> fmt, Args&&... args)
    {
        if (!isEnabled) return false;
        try
        {
            std::string formatted = std::format(fmt, std::forward<Args>(args)...);
            std::string final_msg = std::format("[Aurora] {}", formatted);
            return NotificationModule_AddErrorNotification(final_msg.c_str()) != NOTIFICATION_MODULE_RESULT_SUCCESS;
        }
        catch (const std::format_error& e)
        {
            std::string formatted = std::format("[Aurora][format error] {}", e.what());
            NotificationModule_AddErrorNotification(formatted.c_str());
            return false;
        }
    }
}