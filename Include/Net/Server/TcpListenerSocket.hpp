#pragma once

#include <memory>
#include <optional>

#include "Net/Server/IListenerSocket.hpp"

namespace Server
{
    class TcpListenerSocket : public IListenerSocket
    {
    private:
        std::optional<int> fd;
        TcpListenerSocket(int fd);

    public:
        ~TcpListenerSocket();

        static std::unique_ptr<TcpListenerSocket> create(uint16_t port);

        std::unique_ptr<IConnectionSocket> accept() override;
    };
}