#pragma once

#include <memory>
#include <optional>

#include "Net/Transport/IListenerSocket.hpp"

namespace Net::Transport
{
    class TcpListenerSocket : public IListenerSocket
    {
    private:
        std::optional<int> fd;
        TcpListenerSocket(int fd);

    public:
        ~TcpListenerSocket();

        static std::unique_ptr<TcpListenerSocket> create(uint16_t port);

        std::shared_ptr<IConnectionSocket> accept(std::function<void(std::shared_ptr<IConnectionSocket>, std::vector<std::byte>)> onReceive) override;
    };
}