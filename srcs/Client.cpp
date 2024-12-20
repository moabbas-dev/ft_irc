#include "../includes/Client.hpp"

Client::Client() {

}

int Client::getFd() {
	return fd;
}

void Client::setFd(int fd) {
	this->fd = fd;
}

std::string Client::getIPadd() {
	return IPadd;
}

void Client::setIPadd(std::string IPadd) {
	this->IPadd = IPadd;
}
