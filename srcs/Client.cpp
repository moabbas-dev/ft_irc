#include "../includes/Client.hpp"

Client::Client() : fd(-1), isAuthenticated(false) {
}

// Getters

int Client::getFd() const {
    return fd;
}

std::string Client::getNickname() const {
    return nickname;
}

std::string Client::getUsername() const {
    return username;
}

std::string Client::getIPadd() const {
    return IPadd;
}

bool Client::getIsAuthenticated() const {
    return isAuthenticated;
}

// Setters

void Client::setFd(int fd) {
    this->fd = fd;
}

void Client::setNickname(const std::string& nickname) {
    this->nickname = nickname;
}

void Client::setUsername(const std::string& username) {
    this->username = username;
}

void Client::setIPadd(const std::string& IPadd) {
    this->IPadd = IPadd;
}

void Client::setIsAuthenticated(bool isAuthenticated) {
    this->isAuthenticated = isAuthenticated;
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
