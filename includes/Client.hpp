#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>

class Client {
	private:
		int fd;
		std::string IPadd;
		std::string nickname;
		std::string username;
		bool isAuthenticated;
		std::vector<std::string> channels;
		std::string messageBuffer;
	public:
		Client();
		int getFd() const;
		std::string getNickname() const;
		void setFd(int fd);
		std::string getIPadd() const ;
		void setIPadd(std::string IPadd);
		bool isOperator(const std::string& channel);
		void joinChannel(const std::string& channel);
		void leaveChannel(const std::string& channel);
};

#endif