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
    // Check if the client is an operator in the specified channel.
    for (std::vector<std::string>::const_iterator it = channels.begin(); it != channels.end(); ++it) {
        if (*it == channel) {
            return true; // Assume the channel name indicates operator status
        }
    }
    return false;
}

void Client::joinChannel(const std::string& channel) {
    // Add the channel to the list if it's not already present.
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
        if (*it == channel) {
            return; // Already in the channel
        }
    }
    channels.push_back(channel);
}

void Client::leaveChannel(const std::string& channel) {
    // Remove the channel from the list if it exists.
    for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
        if (*it == channel) {
            channels.erase(it);
            return;
        }
    }
}
