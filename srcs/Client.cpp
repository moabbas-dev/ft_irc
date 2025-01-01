/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 12:53:55 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/01 12:13:26 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client() : fd(-1), isAuthenticated(false), hasSetPassword(false),
    hasSetNickName(false), hasSetUser(false) { }

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

std::string Client::getRealname() const {
    return realname;
}

std::string Client::getIPadd() const {
    return IPadd;
}

bool Client::getIsAuthenticated() const {
    return isAuthenticated;
}

std::string Client::getHostName() const {
    return hostName;
}

std::list<Cmd> Client::getCommands() const {
    return commands;
}

bool Client::getHasSetPassword() const
{
    return hasSetPassword;
}

bool Client::getHasSetNickName() const
{
    return hasSetNickName;
}

bool Client::getHasSetUser() const
{
    return hasSetUser;
}

std::vector<Channel>& Client::getChannels() {
    return channels;
}

void Client::clearTempChannels()  {
    tmp_channels.clear();
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

void Client::setRealname(const std::string& realname) {
    this->realname = realname;
}

void Client::setIPadd(const std::string& IPadd) {
    this->IPadd = IPadd;
}

void Client::setIsAuthenticated(bool isAuthenticated) {
    this->isAuthenticated = isAuthenticated;
}

void Client::setHostName(std::string hostName) {
    this->hostName = hostName;
}

void Client::setCommands(std::list<Cmd> commands) {
    this->commands = commands;
}

void Client::setHasSetPassword(bool hasSetPassword)
{
    this->hasSetPassword = hasSetPassword;
}

void Client::setHasSetNickName(bool hasSetNickName)
{
    this->hasSetNickName = hasSetNickName;
}

void Client::setHasSetUser(bool hasSetUser)
{
    this->hasSetUser = hasSetUser;
}

void Client::setChannels(std::vector<Channel> channels) {
    this->channels = channels;
}

bool Client::isOperator(Channel& channel) const {
    for (std::vector<Channel>::const_iterator it = channels.begin(); it != channels.end(); ++it)
        if ((*it).getName() == channel.getName())
            return true;
    return false;
}

// void Client::joinChannel(Channel& channel) {
//     for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
//         if (*it == channel)
//             return;
//     }
//     channels.push_back(channel);
// }

// void Client::leaveChannel(const std::string& channel) {
//     for (std::vector<std::string>::iterator it = channels.begin(); it != channels.end(); ++it) {
//         if (*it == channel) {
//             channels.erase(it);
//             return;
//         }
//     }
// }

std::vector<Channel> Client::getTempChannels() const {
    return tmp_channels;
}

void Client::setTempChannels(std::vector<Channel> tmp_channels) {
    this->tmp_channels = tmp_channels;
}
