#pragma once

// std
#include <string>
#include <format>

// wut
#include <coreinit/context.h>
#include <coreinit/debug.h>

// this
#include <Util/Singleton.hpp>

namespace Aurora::Sys::Exception
{
    template <typename T>
    class Base : public Util::Singleton<T>
    {
    public:
        virtual std::string name() = 0;

        virtual void panic(std::string name, OSContext* context)
        {
            std::string message = std::format("{0} Exception occurred from 0x{1:08X} in 0x{2:08X}", name, context->srr0, context->dar);
            OSFatal(message.c_str());
        }

        virtual bool handler(OSContext* context)
        {
            panic(name(), context);
            return false;
        }

        static BOOL callback(OSContext* context)
        {
            return static_cast<BOOL>(T::instance().handler(context));
        }
    };
}