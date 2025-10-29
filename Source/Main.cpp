// wups
#include <wups.h>

// wut
#include <coreinit/launch.h>
#include <coreinit/debug.h>

// this
#include "Core.hpp"
#include "Sys/Exception/Patch.hpp"
#include "Sys/Debug/Logger.hpp"
#include "Sys/Debug/Notification.hpp"

WUPS_PLUGIN_NAME("Aurora");
WUPS_PLUGIN_DESCRIPTION("Enables WiiU control using commands");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Nanashi");
WUPS_PLUGIN_LICENSE("None");

WUPS_USE_WUT_DEVOPTAB(); // ファイルの読み書きに必須(無いとフリーズ)
WUPS_USE_STORAGE("Aurora"); // ON_APPLICATION_REQUESTS_EXITに必要

INITIALIZE_PLUGIN()
{
    Aurora::Sys::Debug::Notification::init();
    Aurora::Sys::Exception::SetExceptionCallbacks();
}

ON_APPLICATION_START()
{
    Aurora::Sys::Debug::Logger::init();

    Aurora::Core::run();
}

ON_APPLICATION_REQUESTS_EXIT()
{
    Aurora::Core::stop();

    Aurora::Sys::Debug::Logger::shutdown();
}