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
#include "wups/function_patching.h"

WUPS_PLUGIN_NAME("Aurora");
WUPS_PLUGIN_DESCRIPTION("Enables WiiU control using commands");
WUPS_PLUGIN_VERSION("v1.0");
WUPS_PLUGIN_AUTHOR("Nanashi");
WUPS_PLUGIN_LICENSE("None");

WUPS_USE_WUT_DEVOPTAB(); // ファイルの読み書きに必須(無いとフリーズ)
WUPS_USE_STORAGE("Aurora"); // ON_APPLICATION_REQUESTS_EXITに必要

#include <coreinit/ios.h>
#include <coreinit/thread.h>
#include <mocha/mocha.h>

#define IOCTL_SVC 0x02

int haxHandle;

extern "C"
{
    void OSSendAppSwitchRequest(uint32_t rampid, void* args, uint32_t argsSize);
}

DECL_FUNCTION(void, PPCHalt)
{
    OSSendAppSwitchRequest(0,0,0);
    OSSleepTicks(OSMillisecondsToTicks(5000));
    __attribute__((aligned(0x40))) uint32_t arguments[0x40 >> 2];
    arguments[0] = 0x74;

    __attribute__((aligned(0x40))) int result[0x40 >> 2];
    IOS_Ioctl(haxHandle, IOCTL_SVC, arguments, (1 + 0) * 4, result, 4);
    return;
}

INITIALIZE_PLUGIN()
{
    Debug::Notification::init();
    Exception::Patch::apply();
}

std::unique_ptr<Net::Transport::Server> gServer;

ON_APPLICATION_START()
{
    Debug::Logger::init();

    haxHandle = IOS_Open("/dev/iosuhax", static_cast<IOSOpenMode>(0));
    Mocha_InitLibrary();

    gServer = std::make_unique<Net::Transport::Server>(55555);
    gServer->start();
}

ON_APPLICATION_REQUESTS_EXIT()
{
    Mocha_DeInitLibrary();

    gServer.reset();
    Debug::Logger::shutdown();
}

WUPS_MUST_REPLACE_FOR_PROCESS(PPCHalt, WUPS_LOADER_LIBRARY_COREINIT, PPCHalt, WUPS_FP_TARGET_PROCESS_ALL);