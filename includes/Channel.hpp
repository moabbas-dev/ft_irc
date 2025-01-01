#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include "Client.hpp"
#include <sys/socket.h>
#include <sstream>

#define MAX_CHANNEL_NAME_LENGTH 200

class Client;

enum CommandType {
    KICK,
    INVITE,
    MODE,
    SET_TOPIC,
    SHOW_TOPIC
};

class Channel {
private:
    std::string name;
    std::string topic;
    std::string password;
    std::string channelKey;
    std::vector<Client> clients;
    std::map<int, bool> operators;
    std::map<char, bool> mode;
    int userLimit;
    bool inviteOnly;
    bool topicRestricted;
    bool _hasKey;

public:
    Channel(std::string name, std::string key);
    Channel(std::string name);
    Channel();
    ~Channel();

    std::string getName() const;
    std::string getTopic() const;
    void setTopic(const std::string& topic);
    const std::string& getChannelKey() const;
    const std::map<int, bool> &getOperators() const;
    void setChannelKey(const std::string& key);

    bool addClient(Client& client, const std::string& key = "");
    void removeClient(int fd);
    bool isClientInChannel(int fd) const;
    void addOperator(int fd);
    void removeOperator(int fd);
    bool isOperator(int fd) const;
    void broadcastMessage(const std::string& message, int senderFd);
    bool hasKey() const;
    void setHasKey(bool hasKey);

    private:
    void handleKick(int operatorFd, int targetFd);
    void handleInvite(int operatorFd, int targetFd);
    void handleTopic(int operatorFd, const std::string topic);
    void showTopic(int fd);
    void handleMode(int operatorFd, const std::string& mode);

    public:
    void handleCommand(CommandType command, int operatorFd, const std::string& param = "", int targetFd = -1);
};

#endif