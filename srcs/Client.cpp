/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 12:53:55 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/12 10:33:34 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"
#include "../includes/Cmd.hpp"
#include "../includes/Channel.hpp"
#include "../includes/Server.hpp"

Client::Client() : fd(-1), isAuthenticated(false), hasSetPassword(false),
    hasSetNickName(false), hasSetUser(false), tmp_channels(std::vector<Channel>()) { }

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

std::map<std::string, bool> &Client::getInvitationsBox()
{
    return invitationsBox;
}

Channel *Client::getSpecifiedChannel(const std::string &channelName)
{
    std::cout << "TTTT: " << channels.size() <<"\n";
    if (channels.empty())
        return NULL;
    for (std::vector<Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if ((*it).getName() == channelName)
            return &(*it);
    }
    return NULL;
}


void Client::setSpecifiedChannel(Channel& channel) {
    Channel* old_Channel = getSpecifiedChannel(channel.getName());
    if (!old_Channel)
        return ;
    for (size_t i = 0;i < channels.size(); i++) {
        if (channels[i].getName() == old_Channel->getName()) {
            channels.erase(channels.begin() + i);
            channels.push_back(channel);
        }
    }
    
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

void Client::removeChannel(Channel& channel) {
    for (size_t i = 0;i < channels.size();i++) {
        if (channels[i].getName() == channel.getName()) {
            channels.erase(channels.begin() + i);
            break ;
        }
    }
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

void Client::addInvitationToChannel(const std::string &channelName)
{
    invitationsBox[channelName] = true;
}

void Client::removeChannelInvitation(const std::string &channelName)
{
    std::map<std::string, bool>::iterator it = invitationsBox.begin();
    while (it != invitationsBox.end())
    {
        if (it->first == channelName)
        {
            invitationsBox.erase(it);
            break;
        }
        ++it;
    }
}

bool Client::isInvitedTochannel(Channel& channel) {
    return invitationsBox.find(channel.getName()) != invitationsBox.end();
}

std::vector<std::string> Client::getTempKickUsers() const {
    return tmp_kick_users;
}

void Client::clearTempKickUsers() {
    tmp_kick_users.clear();
}

void Client::setTempKickUsers(std::vector<std::string> tmp_kick_users) {
    this->tmp_kick_users = tmp_kick_users;
}
