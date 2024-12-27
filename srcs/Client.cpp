#include "../includes/Client.hpp"

Client::Client() : fd(-1), isAuthenticated(false) {
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

void Client::setIPadd(const std::string& IPadd) {
    this->IPadd = IPadd;
}

bool Client::isOperator(const std::string& channel) const {
    for (std::vector<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it)
        if (*it == channel)
            return true;
    return false;
}

void Client::joinChannel(const std::string& channel) {
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
        if (*it == channel)
            return;
    }
    channels.push_back(channel);
}

void Client::leaveChannel(const std::string& channel) {
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
        if (*it == channel) {
            channels.erase(it);
            return;
        }
    }
}
