#pragma once

#include <cstdint>
#include <memory>
#include <thread>

#include "Net/Protocol/IData.hpp"
#include "Net/Protocol/Registry.hpp"
#include "Net/Transport/Connection.hpp"
#include "Net/Transport/SocketPoller.hpp"
#include "Net/Transport/TcpListenerSocket.hpp"

namespace Net::Transport
{
	class Server
	{
	private:
		int SLEEP_MS = 1000 / 144;

		std::unique_ptr<TcpListenerSocket> listener;
		SocketPoller poller;
		std::thread acceptThread;
		bool isRunning = false;

		std::shared_ptr<Protocol::Registry> registry;
		std::function<void(std::shared_ptr<Protocol::Registry>, std::unique_ptr<Connection>, std::unique_ptr<Protocol::IData>)> dispatcher;

		void acceptLoop();

	public:
		Server(uint16_t port, std::function<void(std::shared_ptr<Protocol::Registry>, std::unique_ptr<Connection>, std::unique_ptr<Protocol::IData>)> dispatcher = nullptr);
		~Server();

		void start();
		void stop();

		void onReceive(std::shared_ptr<IConnectionSocket> socket, std::vector<std::byte> data);
	};
}
