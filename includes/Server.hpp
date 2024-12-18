#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include "Client.hpp"
#include <poll.h>

class Server {
	private:
		int port;
		int serSocketFd;
		static bool isSignalReceived;
		std::vector<Client> clients;
		std::vector<struct pollfd> fds;
	public:
		Server();
		void serverInit();
		void createServerSocket();
		void acceptNewClient();
		void recieveData(int fd);
		static void signalHandler(int signum);
		void closeFds();
		void clearClients(int fd);
};

#endif