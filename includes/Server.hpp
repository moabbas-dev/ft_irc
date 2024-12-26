#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <map>
#include "Client.hpp"
#include "Channel.hpp"
#include <poll.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <csignal>
#include <arpa/inet.h> 
#include <string.h>

class Server {
	private:
		int port;
		int serSocketFd;
		static bool isSignalReceived;
		std::vector<Client> clients;
		std::vector<struct pollfd> fds;
		std::string password;
		std::map<std::string, Channel> channels;

	public:
		Server();
		Server(int port, std::string password);
		void serverInit();
		void createServerSocket();
		void acceptNewClient();
		void recieveData(int fd);
		static void signalHandler(int signum);
		void closeFds();
		void clearClients(int fd);
		void run();
		void handleJoin(int fd, const std::string& channelName);
		void handlePrivmsg(int senderFd, const std::string& target, const std::string& message);
		void handlePart(int fd, const std::string& channelName);
};

#endif
