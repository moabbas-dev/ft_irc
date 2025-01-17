/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:49:30 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/17 14:49:31 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <sstream>
#include <ctime>
#include <stdexcept>

#define MAX_CHANNEL_NAME_LENGTH 200

class Client;

class Channel {
private:
    std::string name;
    std::string topic;
    std::string password;
    std::string channelKey;
    std::vector<Client> clients;
    std::map<int, bool> operators;
    std::map<char, bool> mode;
    std::time_t creationTime;
    int userLimit;
    bool inviteOnly;
    bool topicRestricted;
    bool _hasKey;
    bool hasUserLimit;
    std::time_t topicTime;
    std::string topic_modifier;

public:
    Channel(std::string name, std::string key);
    Channel(std::string name);
    Channel(const Channel &other);
    Channel();
    ~Channel();
    bool operator==(const Channel& other) const;
    // const Channel& operator=(Channel& other) const;

    // getters
    std::string getName() const;
    std::string getTopic() const;
    const std::string& getChannelKey() const;
    int getUserLimit() const;
    bool getHasUserLimit() const;
    const std::map<int, bool> &getOperators() const;
    std::map<int, bool> &getOperators();
    std::time_t getCreationTime() const;
    std::map<char, bool> &getMode();
    std::vector<Client> &getClients();
    std::time_t getTopicTime();
    std::string getTopicModifier();

    // setters
    void setTopic(const std::string& topic);
    void setChannelKey(const std::string& key);
    void setInviteOnly(bool inviteOnly);
    void setTopicRestricted(bool topicRestricted);
    void setUserLimit(int userLimit);
    void setHasUserLimit(bool hasUserLimit);
    void setTopicTime(std::time_t topicTime);
    void setTopicModifier(std::string name);

    // client's methods
    bool addClient(Client& client, const std::string& key = "");
    void removeClient(int fd);
    bool isClientInChannel(int fd) const;

    // operator's methods
    void addOperator(int fd);
    void removeOperator(int fd);
    bool isOperator(int fd) const;

    // channel methods
    bool isEmpty() const;
    void broadcastMessage(const std::string& message, int senderFd) const;
    bool hasKey() const;
    void setHasKey(bool hasKey);
    bool hasTopicRestricions();
    std::string clientslist();
    bool IsInviteOnly();
    bool clientIsInChannel(const std::string &nickname);
    int getClientsNumber();
};

#endif