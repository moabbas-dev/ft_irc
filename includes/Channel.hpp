#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include "Client.hpp"
#include <sys/socket.h>
#include <sstream>

class Channel {
private:
    std::string name;                             // Channel name
    std::string topic;                            // Channel topic
    std::vector<Client> clients;                 // List of clients in the channel
    std::map<int, bool> operators;              // Map client FDs to operator status (true = operator)
    std::map<char, bool> mode;                  // Tracks channel modes (e.g., 'i', 't', etc.)
    std::string password;                        // Channel password (if `k` mode is set)
    int userLimit;                               // User limit (if `l` mode is set)
    bool inviteOnly;                             // Is the channel invite-only (`i` mode)?
    bool topicRestricted;                        // Is topic restricted to operators (`t` mode)?
    std::string channelKey;

public:
    Channel(std::string name, std::string key);
    Channel(std::string name);
    ~Channel();

    // Getters and setters
    std::string getName() const;
    std::string getTopic() const;
    void setTopic(const std::string& topic, int fd); // Restrict based on operator status if `t` mode
    const std::string& getChannelKey() const;
    void setChannelKey(const std::string& key);

    // Client management
    bool addClient(Client& client, const std::string& key = ""); // Check for password and user limit
    void removeClient(int fd);
    bool isClientInChannel(int fd) const;
    void addOperator(int fd);
    void removeOperator(int fd);
    bool isOperator(int fd) const;

    // Broadcast
    void broadcastMessage(const std::string& message, int senderFd);

    // Command handlers
    void handleKick(int operatorFd, int targetFd);
    void handleInvite(int operatorFd, int targetFd);
    void handleMode(int operatorFd, const std::string& mode);
};

#endif