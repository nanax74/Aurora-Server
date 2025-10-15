#pragma once

#include <coreinit/exception.h>

#include "Sys/Exception/Impl.hpp"

namespace CC::Sys::Exception
{
    void SetExceptionCallbacks()
    {
        OSExceptionMode mode = OSExceptionMode::OS_EXCEPTION_MODE_GLOBAL_ALL_CORES;
        OSSetExceptionCallbackEx(mode, OSExceptionType::OS_EXCEPTION_TYPE_SYSTEM_RESET, SystemReset::callback);
        OSSetExceptionCallbackEx(mode, OSExceptionType::OS_EXCEPTION_TYPE_MACHINE_CHECK, MachineCheck::callback);
        OSSetExceptionCallbackEx(mode, OSExceptionType::OS_EXCEPTION_TYPE_DSI, DSI::callback);
        OSSetExceptionCallbackEx(mode, OSExceptionType::OS_EXCEPTION_TYPE_ISI, ISI::callback);
        OSSetExceptionCallbackEx(mode, OSExceptionType::OS_EXCEPTION_TYPE_EXTERNAL_INTERRUPT, ExternalInterrupt::callback);
        OSSetExceptionCallbackEx(mode, OSExceptionType::OS_EXCEPTION_TYPE_ALIGNMENT, Alignment::callback);
        OSSetExceptionCallbackEx(mode, OSExceptionType::OS_EXCEPTION_TYPE_PROGRAM, Program::callback);
        OSSetExceptionCallbackEx(mode, OSExceptionType::OS_EXCEPTION_TYPE_FLOATING_POINT, FloatingPoint::callback);
        OSSetExceptionCallbackEx(mode, OSExceptionType::OS_EXCEPTION_TYPE_DECREMENTER, Decrementer::callback);
        OSSetExceptionCallbackEx(mode, OSExceptionType::OS_EXCEPTION_TYPE_SYSTEM_CALL, SystemCall::callback);
        OSSetExceptionCallbackEx(mode, OSExceptionType::OS_EXCEPTION_TYPE_TRACE, Trace::callback);
        OSSetExceptionCallbackEx(mode, OSExceptionType::OS_EXCEPTION_TYPE_PERFORMANCE_MONITOR, PerformanceMonitor::callback);
        OSSetExceptionCallbackEx(mode, OSExceptionType::OS_EXCEPTION_TYPE_BREAKPOINT, Breakpoint::callback);
        OSSetExceptionCallbackEx(mode, OSExceptionType::OS_EXCEPTION_TYPE_SYSTEM_INTERRUPT, SystemInterrupt::callback);
        OSSetExceptionCallbackEx(mode, OSExceptionType::OS_EXCEPTION_TYPE_ICI, ICI::callback);
    }
}