#pragma once

#include <cstdint>
#include <memory>
#include <thread>

#include "Net/PollManager.hpp"
#include "Net/TcpListenerSocket.hpp"

namespace Net
{
	class Server
	{
	private:
		std::unique_ptr<TcpListenerSocket> listener;
		PollManager pollManager;
		std::thread acceptThread;
		bool isRunning = false;

		void acceptLoop();

	public:
		Server(uint16_t port);
		~Server();

		void start();
		void stop();
	};
}
