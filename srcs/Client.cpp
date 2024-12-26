#include "../includes/Client.hpp"

Client::Client() {

}

int Client::getFd() const {
	return fd;
}

void Client::setFd(int fd) {
	this->fd = fd;
}

std::string Client::getNickname() const {
	return nickname;
}

std::string Client::getIPadd() const {
	return IPadd;
}

void Client::setIPadd(std::string IPadd) {
	this->IPadd = IPadd;
}
