// wups
#include <wups.h>

// wut
#include <coreinit/launch.h>
#include <coreinit/debug.h>

// this
#include "Net/Protocol/Processor.hpp"
#include "Net/Transport/Server.hpp"
#include "Exception/Patch.hpp"
#include "Debug/Logger.hpp"
#include "Debug/Notification.hpp"

WUPS_PLUGIN_NAME("Aurora");
WUPS_PLUGIN_DESCRIPTION("Enables WiiU control using commands");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Nanashi");
WUPS_PLUGIN_LICENSE("None");

WUPS_USE_WUT_DEVOPTAB(); // ファイルの読み書きに必須(無いとフリーズ)
WUPS_USE_STORAGE("Aurora"); // ON_APPLICATION_REQUESTS_EXITに必要

INITIALIZE_PLUGIN()
{
    Debug::Notification::init();
    Exception::Patch::apply();
}

std::unique_ptr<Net::Transport::Server> gServer;

ON_APPLICATION_START()
{
    Debug::Logger::init();
    gServer = std::make_unique<Net::Transport::Server>(55555);
    gServer->start();
}

ON_APPLICATION_REQUESTS_EXIT()
{
    gServer.reset();
    Debug::Logger::shutdown();
}