// std
#include <cstdint>

// wups
#include <wups.h>

// wut
#include <coreinit/launch.h>
#include <coreinit/debug.h>

// this
#include "Sys/Exception/Patch.hpp"
#include "Sys/Debug/Logger.hpp"
#include "Sys/Debug/Notification.hpp"

WUPS_PLUGIN_NAME("Stardust");
WUPS_PLUGIN_DESCRIPTION("TCPServer");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Nanashi");
WUPS_PLUGIN_LICENSE("GPLv3");

WUPS_USE_WUT_DEVOPTAB(); // ファイルの読み書きに必須(無いとフリーズ)
WUPS_USE_STORAGE("Stardust"); // ON_APPLICATION_REQUESTS_EXITに必要

INITIALIZE_PLUGIN()
{
    Aurora::Sys::Debug::Notification::init();
    Aurora::Sys::Exception::SetExceptionCallbacks();
}

ON_APPLICATION_START()
{
    Aurora::Sys::Debug::Logger::init();
}

ON_APPLICATION_REQUESTS_EXIT()
{
    Aurora::Sys::Debug::Logger::shutdown();
}