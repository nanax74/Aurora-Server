#pragma once

#include <memory>
#include <functional>

#include <Net/Socket/Interface/Stream.hpp>

namespace Aurora::Net::Transport
{
    class Connection
    {
    private:
        std::unique_ptr<Socket::Interface::Stream> stream;

    public:
        std::function<void(Connection*)> onDisconnect;

        explicit Connection(std::unique_ptr<Socket::Interface::Stream> stream) : stream(std::move(stream)) {}

        std::optional<int> recv(std::vector<uint8_t>& buffer)
        {
            if (!stream) return std::nullopt;
            auto ret = stream->recv(buffer);
            if(!ret && onDisconnect) onDisconnect(this);
            return ret;
        }

        bool send(const std::vector<uint8_t>& buffer)
        {
            stream->send(buffer);
            return false;
        }

        std::optional<Socket::Endpoint> local()  { return stream->getLocalEndpoint();  }
        std::optional<Socket::Endpoint> remote() { return stream->getRemoteEndpoint(); }

        bool isClosed() const
        {
            auto closed = stream->isClosed();
            return closed.value_or(true);
        }
    };
}