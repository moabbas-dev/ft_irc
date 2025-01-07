/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 12:53:55 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/06 16:28:42 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client() : fd(-1), isAuthenticated(false), hasSetPassword(false),
    hasSetNickName(false), hasSetUser(false) { }

Client::~Client() {
    
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
    return this->getNickname() + "!" + this->getUsername();
}

std::string Client::getoriginalhostname() const {
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

std::vector<Channel> Client::getTempChannels() const {
    return tmp_channels;
}

void Client::setTempChannels(std::vector<Channel> tmp_channels) {
    this->tmp_channels = tmp_channels;
}

bool Client::isOperator(Channel& channel) const {
    for (std::vector<Channel>::const_iterator it = channels.begin(); it != channels.end(); ++it)
        if ((*it).getName() == channel.getName())
            return true;
    return false;
}

void Client::clearTempChannels()  {
    tmp_channels.clear();
}

bool Client::isInsideTheChannel(const std::string &channelName)
{
    for (size_t i = 0; i < channels.size(); ++i)
	{
		if (channelName == channels[i].getName())
			return true;
	}
	return false;
}

bool Client::isOperatorInChannel(const std::string &channelName, Server &server)
{
    std::map<std::string, Channel> &serverChannels = server.getChannels();
    for (std::map<std::string, Channel>::iterator it = serverChannels.begin(); it != serverChannels.end(); ++it)
    {
        if (it->first == channelName)
        {
            const std::map<int, bool> &operators = it->second.getOperators();
            std::map<int, bool>::const_iterator op_it = operators.begin();
            while (op_it != operators.end())
            {
                if (op_it->first == this->getFd())
                {
                    if (op_it->second)
                        return true;
                }
                ++op_it;
            }
        }
    }
    return false;
}