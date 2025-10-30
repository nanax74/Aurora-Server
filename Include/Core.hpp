#pragma once

#include "Net/Transport/Connection.hpp"
#include "Net/Transport/ConnectionManager.hpp"
#include "Net/Transport/Acceptor.hpp"
#include "Net/Transport/Dispatcher.hpp"
#include "Sys/Debug/Logger.hpp"
#include "Util/Singleton.hpp"
#include <vector>

namespace Aurora
{
    class Core : public Util::Singleton<Core>
    {
    private:
        static void processor(Net::Transport::Connection& connection, std::vector<uint8_t> buffer)
        {
            std::string str(reinterpret_cast<char*>(buffer.data()), buffer.size());
            Sys::Debug::Logger::printf("Received: {}", str);

            std::string msg("Welcome to WiiU!\0Extra", 19); // 任意のバイト列
            std::vector<uint8_t> vec(msg.begin(), msg.end());
            connection.send(vec);
        }

        uint16_t port = 55555;    

        Net::Transport::ConnectionManager connectionManager;
        Net::Transport::Acceptor acceptor = Net::Transport::Acceptor(port, connectionManager);
        Net::Transport::Dispatcher dispatcher = Net::Transport::Dispatcher(connectionManager, processor);

        void start()
        {
            acceptor.run();
            dispatcher.run();
        }

        void end()
        {
            dispatcher.stop();
            acceptor.stop();
        }

    public:
        static void run()
        {
            instance().start();
        }

        static void stop()
        {
            instance().end();
        }
    };
}