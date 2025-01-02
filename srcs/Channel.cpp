/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/29 12:53:48 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/01 20:15:47 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "../includes/Errors.hpp"

Channel::Channel(): name(""), channelKey("") {}

Channel::Channel(std::string name, std::string key): name(name), channelKey(key)
{
    creationTime = time(NULL);
    mode['i'] = false;
    mode['t'] = false;
    mode['k'] = false;
    mode['o'] = false;
    mode['l'] = false;
    _hasKey = !key.empty();
}

Channel::Channel(std::string name): name(name), channelKey("")
{
    creationTime = time(NULL);
    mode['i'] = false;
    mode['t'] = false;
    mode['k'] = false;
    mode['o'] = false;
    mode['l'] = false;
    _hasKey = false;
}

Channel::~Channel()
{
    creationTime = time(NULL);
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

void Channel::setChannelKey(const std::string& key) {
    channelKey = key;
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
        if (clients[i].getFd() == client.getFd()) {
            Errors::raise(client, this->name, ERR_ALREADYREGISTERED);
            return false;
        }
    }
    clients.push_back(client);
    return true;
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
        std::cout << "Client <" << fd << "> removed from operators list." << std::endl;
    } else
        std::cout << "Client <" << fd << "> is not an operator." << std::endl;
}

void Channel::addOperator(int fd) {
    if (operators.find(fd) != operators.end() && operators[fd]) {
        return;
    }

    operators[fd] = true;
}

void Channel::removeOperator(int fd) {
    std::map<int, bool>::iterator it = operators.find(fd);
    if (it == operators.end() || !it->second) {
        std::cout << "Client <" << fd << "> is not an operator." << std::endl;
        return;
    }
    operators.erase(it);
    std::cout << "Client <" << fd << "> has been removed as an operator." << std::endl;
}

void Channel::broadcastMessage(const std::string& message, int senderFd) {
    for (size_t i = 0;i < clients.size(); i++)
        if (clients[i].getFd() != senderFd)
            if (send(clients[i].getFd(), message.c_str(), message.size(), 0) == -1) 
                std::cerr << "Send failed to client <" << clients[i].getFd() << ">" << std::endl;
}

// /connect localhost 8000 pass jfatfat

void Channel::handleKick(int operatorFd, int targetFd) {
    if (!isOperator(operatorFd)) {
        std::string errorMsg = "Error: You do not have the permission to kick users.\n";
        send(operatorFd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    if (!isClientInChannel(targetFd)) {
        std::string errorMsg = "Error: The specified user is not in the channel.\n";
        send(operatorFd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }
    std::string kickMsg = "You have been kicked from the channel by an operator.\n";
    send(targetFd, kickMsg.c_str(), kickMsg.size(), 0);

    std::ostringstream oss;
    oss << "User " << targetFd 
    << " has been kicked from the channel by Operator " 
    << operatorFd << ".\n";
    std::string broadcastMsg = oss.str();
    broadcastMessage(broadcastMsg, operatorFd);

    removeClient(targetFd);
}

void Channel::handleInvite(int operatorFd, int targetFd) {
    if (!isOperator(operatorFd)) {
        std::string errorMsg = "Error: You do not have the permission to invite users.\n";
        send(operatorFd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    if (isClientInChannel(targetFd)) {
        std::string errorMsg = "Error: The user is already a member of the channel.\n";
        send(operatorFd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    if (mode['i'] && !isOperator(operatorFd)) {
        std::string errorMsg = "Error: The channel is invite-only. Only operators can invite users.\n";
        send(operatorFd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    std::ostringstream os;
    os << "You have been invited to join the channel by Operator " << operatorFd << ".\n";
    std::string inviteMsg = os.str();
    send(targetFd, inviteMsg.c_str(), inviteMsg.size(), 0);

    std::ostringstream oss;
    oss << "User " << targetFd << " has been invited to the channel.\n";
    std::string confirmMsg = oss.str();
    send(operatorFd, confirmMsg.c_str(), confirmMsg.size(), 0);
}

// !!!---WE HAVE SOME RESERVATIONS ON THIS FUNCTION---!!!
void Channel::handleMode(int operatorFd, const std::string& mode) {
    if (!isOperator(operatorFd)) {
        std::cerr << "Error: Only operators can change channel modes." << std::endl;
        return ;
    }

    bool addMode = true;
    size_t i = 0;

    while (i < mode.size()) {
        char flag = mode[i];
        if (flag == '+')
            addMode = true;
        else if (flag == '-')
            addMode = false;
        else if (mode.find(flag) != std::string::npos) {
            switch (flag) {
            case 'i':
            case 't':
            case 'o':
                this->mode[flag] = addMode;
                break;
            case 'k':
                if (addMode) {
                    if (i + 1 >= mode.size())
                        throw std::runtime_error("Error: Key mode requires an argument.");
                    this->channelKey = mode.substr(i + 1);
                    i = mode.size();
                } else {
                    this->channelKey.clear();
                }
                break;

            case 'l':
                if (addMode) {
                    if (i + 1 >= mode.size())
                        std::cerr << "Error: Limit mode requires an argument.\n";
                    std::istringstream iss(mode.substr(i + 1));
                    int value;
                    if (iss >> value)
                        this->userLimit = value;
                    else
                        throw std::invalid_argument("Invalid integer format");
                    i = mode.size();
                } else
                    this->userLimit = 0;
                break;
            default:
                throw std::runtime_error("Error: Invalid mode flag.");
            }
        }

        ++i;
    }

    std::string modeChange = "Channel mode updated: " + mode;
    broadcastMessage(modeChange, operatorFd);
}

void Channel::handleTopic(int operatorFd, const std::string topic) {
	if (topicRestricted && !isOperator(operatorFd)) {
        std::cerr << "Permission denied: Only operators can set the topic.\n";
        return;
    }
    std::string oldTopic(this->topic);
    setTopic(topic);
    std::ostringstream os;
    os << "Channel topic has been changed from: " << oldTopic +" to: " << topic << std::endl;
    broadcastMessage(os.str(), operatorFd);
}

void Channel::showTopic(int fd) {
    std::ostringstream os;
    os << "Channel <" << this->name << "> has topic: <" << this->topic << ">" << std::endl;
    send(fd, os.str().c_str(), sizeof(os.str().size()), 0);
}

bool Channel::hasKey() const {
    return !this->channelKey.empty();
}

void Channel::setHasKey(bool hasKey) {
    this->_hasKey = hasKey;
}

void Channel::handleCommand(CommandType command, int operatorFd, const std::string& param, int targetFd) {
    switch (command) {
        case KICK:
            handleKick(operatorFd, targetFd);
            break;
        case INVITE:
            handleInvite(operatorFd, targetFd);
            break;
        case MODE:
            handleMode(operatorFd, param);
            break;
        case SET_TOPIC:
            handleTopic(operatorFd, param);
            break;
        case SHOW_TOPIC:
            showTopic(operatorFd);
            break;
        default:
            throw std::invalid_argument("Invalid command type");
    }
}