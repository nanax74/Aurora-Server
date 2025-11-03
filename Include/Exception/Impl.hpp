#pragma once

#include <deque>

#include "Exception/Base.hpp"
#include "coreinit/context.h"
#include "coreinit/exception.h"

namespace Exception
{
    class SystemReset : public Base
    {
    public:
        std::string name() override;
        OSExceptionType type() override;
    };

    class MachineCheck : public Base
    {
    public:
        std::string name() override;
        OSExceptionType type() override;
    };

    class DSI : public Base
    {
    private:
        std::deque<OSContext> queue;

    public:
        std::string name() override;
        OSExceptionType type() override;

        void panic(std::string name, OSContext* context) override;
        bool handle(OSContext* context) override;
    };

    class ISI : public Base
    {
    public:
        std::string name() override;
        OSExceptionType type() override;
    };

    class ExternalInterrupt : public Base
    {
    public:
        std::string name() override;
        OSExceptionType type() override;
    };

    class Alignment : public Base
    {
    public:
        std::string name() override;
        OSExceptionType type() override;
    };

    class Program : public Base
    {
    public:
        std::string name() override;
        OSExceptionType type() override;
    };

    class FloatingPoint : public Base
    {
    public:
        std::string name() override;
        OSExceptionType type() override;
    };

    class Decrementer : public Base
    {
    public:
        std::string name() override;
        OSExceptionType type() override;
    };

    class SystemCall : public Base
    {
    public:
        std::string name() override;
        OSExceptionType type() override;
    };

    class Trace : public Base
    {
    public:
        std::string name() override;
        OSExceptionType type() override;
    };

    class PerformanceMonitor : public Base
    {
    public:
        std::string name() override;
        OSExceptionType type() override;
    };

    class Breakpoint : public Base
    {
    public:
        std::string name() override;
        OSExceptionType type() override;
    };

    class SystemInterrupt : public Base
    {
    public:
        std::string name() override;
        OSExceptionType type() override;
    };

    class ICI : public Base
    {
    public:
        std::string name() override;
        OSExceptionType type() override;
    };
}