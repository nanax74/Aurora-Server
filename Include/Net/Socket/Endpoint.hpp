#pragma once

#include <cstdint>

namespace Aurora::Net::Socket
{
    struct Endpoint
    {
        uint32_t ip;
        uint16_t port;
    };
}