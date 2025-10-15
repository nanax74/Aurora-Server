#pragma once

// std
#include <deque>

// this
#include "Sys/Exception/Base.hpp"
#include "Util/Singleton.hpp"

namespace Aurora::Sys::Exception
{
    std::deque<OSContext> queue;

    class SystemReset : public Base<SystemReset>
    {
    public:
        std::string name() override
        {
            return "SystemReset";
        }
    };

    class MachineCheck : public Base<MachineCheck>
    {
    public:
        std::string name() override
        {
            return "MachineCheck";
        }
    };

    class DSI : public Base<DSI>
    {
    public:
        bool handler(OSContext* context) override
        {
            // DABR による例外でなければクラッシュ
            if ((context->dsisr & (1 << 22)) == 0)
            {
                // メッセージを表示してクラッシュ
                panic("DSI", context);
                return false;
            }
            else
            {
                // 通知の準備
                queue.push_back(*context);

                // 復帰処理
                context->srr0 += 4;
                return true;
            }
        }

        std::string name() override
        {
            return "DSI";
        }
    };

    class ISI : public Base<ISI>
    {
    public:
        std::string name() override
        {
            return "ISI";
        }
    };

    class ExternalInterrupt : public Base<ExternalInterrupt>
    {
    public:
        std::string name() override
        {
            return "ExternalInterrupt";
        }
    };

    class Alignment : public Base<Alignment>
    {
    public:
        std::string name() override
        {
            return "Alignment";
        }
    };

    class Program : public Base<Program>
    {
    public:
        std::string name() override
        {
            return "Program";
        }
    };

    class FloatingPoint : public Base<FloatingPoint>
    {
    public:
        std::string name() override
        {
            return "FloatingPoint";
        }
    };

    class Decrementer : public Base<Decrementer>
    {
    public:
        std::string name() override
        {
            return "Decrementer";
        }
    };

    class SystemCall : public Base<SystemCall>
    {
    public:
        std::string name() override
        {
            return "SystemCall";
        }
    };

    class Trace : public Base<Trace>
    {
    public:
        std::string name() override
        {
            return "Trace";
        }
    };

    class PerformanceMonitor : public Base<PerformanceMonitor>
    {
    public:
        std::string name() override
        {
            return "PerformanceMonitor";
        }
    };

    class Breakpoint : public Base<Breakpoint>
    {
    public:
        std::string name() override
        {
            return "Breakpoint";
        }
    };

    class SystemInterrupt : public Base<SystemInterrupt>
    {
    public:
        std::string name() override
        {
            return "SystemInterrupt";
        }
    };

    class ICI : public Base<ICI>
    {
    public:
        std::string name() override
        {
            return "ICI";
        }
    };
}