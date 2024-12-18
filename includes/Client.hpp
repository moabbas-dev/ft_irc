#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client {
	private:
		int fd;
		std::string IPadd;
	public:
		Client();
		int getFd();
		void setFd(int fd);
		std::string getIPadd();
		void setIPadd(std::string IPadd);
};

#endif