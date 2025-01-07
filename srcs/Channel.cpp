/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 12:53:48 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/07 18:10:25 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "../includes/Errors.hpp"

Channel::Channel(): name(""), channelKey("") {}

Channel::Channel(std::string name, std::string key): name(name), channelKey(key)
{
    creationTime = time(NULL);
    topicTime =  time(NULL);
    mode['i'] = false;
    mode['t'] = false;
    mode['k'] = false;
    mode['l'] = false;
    userLimit = -1;
    hasUserLimit = false;
    _hasKey = !key.empty();
    inviteOnly = false;
}

Channel::Channel(std::string name): name(name), channelKey("")
{
    creationTime = time(NULL);
    topicTime = std::time(NULL);
    mode['i'] = false;
    mode['t'] = false;
    mode['k'] = false;
    mode['l'] = false;
    userLimit = -1;
    hasUserLimit = false;
    _hasKey = false;
    inviteOnly = false;
}

Channel::Channel(const Channel &other): 
    name(other.name), topic(other.topic), password(other.password),
    channelKey(other.channelKey), clients(other.clients), operators(other.operators),
    mode(other.mode), creationTime(other.creationTime), userLimit(other.userLimit),
    inviteOnly(other.inviteOnly), topicRestricted(other.topicRestricted),
    _hasKey(other._hasKey) {
    
}

Channel::~Channel()
{
    creationTime = time(NULL);
    topicTime = std::time(NULL);
}

std::string Channel::getName() const {
	return name;
}

std::string Channel::getTopic() const {
	return topic;
}

std::map<char, bool> &Channel::getMode()
{
    return mode;
}

std::time_t Channel::getCreationTime() const
{
    return creationTime;
}

int Channel::getUserLimit() const
{
    return userLimit;
}

bool Channel::getHasUserLimit() const
{
    return hasUserLimit;
}

std::map<int, bool> &Channel::getOperators()
{
    return operators;
}

void Channel::setTopic(const std::string& topic) {
    this->topic = topic;	
}

const std::string& Channel::getChannelKey() const {
    return channelKey;
}

const std::map<int, bool> &Channel::getOperators() const
{
    return operators;
}

std::vector<Client> &Channel::getClients()
{
    return clients;
}

void Channel::setChannelKey(const std::string& key) {
    channelKey = key;
}

void Channel::setInviteOnly(bool inviteOnly)
{
    this->inviteOnly = inviteOnly;
}

void Channel::setTopicRestricted(bool topicRestricted)
{
    this->topicRestricted = topicRestricted;
}

void Channel::setUserLimit(int userLimit)
{
    this->userLimit = userLimit;
}

void Channel::setHasUserLimit(bool hasUserLimit)
{
    this->hasUserLimit = hasUserLimit;
}

bool Channel::hasTopicRestricions() {
    return topicRestricted;
}

bool Channel::isClientInChannel(int fd) const {
	for (size_t i = 0;i < clients.size();i++) {
        if (clients.at(i).getFd() == fd)
            return true;
    }
    return false;
}

bool Channel::isOperator(int fd) const {
    std::map<int, bool>::const_iterator it = operators.find(fd);
    return it != operators.end() && it->second;
}

bool Channel::addClient(Client& client, const std::string& key) {
    (void)key;
    for (size_t i = 0;i < clients.size();i++) {
        if (clients[i].getFd() == client.getFd())
            return false;
    }
    clients.push_back(client);
    return true;
}

bool Channel::isEmpty() const {
    return clients.empty();
}

void Channel::removeClient(int fd) {
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i].getFd() == fd) {
            clients.erase(clients.begin() + i);
            break;
        }
    }

    std::map<int, bool>::iterator it = operators.find(fd);
    if (it != operators.end()) {
        operators.erase(it);
        std::ostringstream oss;
        oss << "Operator <" << fd << "> removed from the channel "<< name <<".";
        Server::printResponse(oss.str(), RED);
    }
}

void Channel::addOperator(int fd) {
    if (operators.find(fd) != operators.end() && operators[fd]) 
        return;

    operators[fd] = true;
}

void Channel::removeOperator(int fd) {
    std::map<int, bool>::iterator it = operators.find(fd);
    if (it == operators.end() || !it->second)
        return;
    operators.erase(it);
    std::ostringstream oss;
    oss << "Client <" << fd << "> has been removed as an operator." << std::endl;
    Server::printResponse(oss.str(), RED);
}

void Channel::broadcastMessage(const std::string& message, int senderFd) const {
    for (size_t i = 0;i < clients.size(); i++)
        if (clients[i].getFd() != senderFd)
            if (send(clients[i].getFd(), message.c_str(), message.size(), 0) == -1) 
                std::cerr << "Send failed to client <" << clients[i].getFd() << ">" << std::endl;
}

bool Channel::hasKey() const {
    return !this->channelKey.empty();
}

void Channel::setHasKey(bool hasKey) {
    this->_hasKey = hasKey;
}

bool Channel::operator==(const Channel& other) const {
    return name == other.name;
}

std::string Channel::clientslist() {
    std::string list;
	for(size_t i = 0; i < clients.size(); i++){
        if (isOperator(clients[i].getFd()))
		    list += "@" + clients[i].getNickname();
        else
		    list += clients[i].getNickname();
		if((i + 1) < clients.size())
			list += " ";
	}
	return list;
}

std::time_t Channel::getTopicTime() {
    return topicTime;
}

void Channel::setTopicTime(std::time_t topicTime) {
    this->topicTime = topicTime;
}

std::string Channel::getTopicModifier() {
    return topic_modifier;
}

void Channel::setTopicModifier(std::string name) {
    topic_modifier = name;
}

bool Channel::IsInviteOnly() {
    return this->inviteOnly;

bool Channel::clientIsInChannel(const std::string &nickname)
{
    for (size_t i = 0; i < clients.size(); ++i)
    {
        if (clients[i].getNickname() == nickname)
            return true;
    }
    return false;
}

int Channel::getClientsNumber()
{
    return clients.size();
}