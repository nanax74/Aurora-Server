#pragma once

#include "Net/Socket/Interface/Listener.hpp"
#include "Net/Socket/Client/Invalid.hpp"

#include "Sys/Debug/Logger.hpp"

#include <optional>

namespace Aurora::Net::Socket::Server
{
    class Invalid : public Interface::Listener
    {
    public:
        Invalid() {}
        ~Invalid() {}

        bool bind() override
        {
            Sys::Debug::Logger::printf("Aurora::Net::Socket::Server::Invalid::bind()");
            return false;
        }

        bool listen() override
        {
            Sys::Debug::Logger::printf("Aurora::Net::Socket::Server::Invalid::listen()");
            return false;
        }

        std::unique_ptr<Interface::Stream> accept() override
        {
            Sys::Debug::Logger::printf("Aurora::Net::Socket::Server::Invalid::accept()");
            return std::make_unique<Client::Invalid>();
        }

        bool close() override
        {
            Sys::Debug::Logger::printf("Aurora::Net::Socket::Server::Invalid::close()");
            return false;
        }

        std::optional<int> getFd() override
        {
            Sys::Debug::Logger::printf("Aurora::Net::Socket::Server::Invalid::getFd()");
            return std::nullopt;
        }

        std::optional<Endpoint> getLocalEndpoint() override
        {
            Sys::Debug::Logger::printf("Aurora::Net::Socket::Server::Invalid::getLocalEndpoint()");
            return std::nullopt;
        }

        std::optional<bool> isClosed() override
        {
            Sys::Debug::Logger::printf("Aurora::Net::Socket::Server::Invalid::isClosed()");
            return std::nullopt;
        }
    };
}