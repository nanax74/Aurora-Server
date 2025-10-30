#pragma once

#include "Net/Socket/Interface/Base.hpp"
#include "Net/Socket/Interface/Stream.hpp"

#include <memory>

namespace Aurora::Net::Socket::Interface
{
    class Listener : public Base
    {
    public:
        virtual bool bind() = 0;
        virtual bool listen() = 0;
        virtual std::unique_ptr<Stream> accept() = 0;
    };
}