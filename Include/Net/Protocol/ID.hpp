#pragma once

namespace Net::Protocol
{
    enum class ID
    {
        Ping = 0,
        Pong = 1,
    
        Invalid = -1
    };
}