#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include "Cmd.hpp"
class Cmd;

class Client {
private:
    int fd;
    std::string IPadd;
    std::string nickname;
    std::string username;
    bool isAuthenticated;
    std::vector<std::string> channels;
    std::string messageBuffer;
    std::string hostName;
    std::list<Cmd> commands;

public:
    Client();
    // Getters
    int getFd() const;
    std::string getNickname() const;
    std::string getUsername() const;
    std::string getIPadd() const;
    bool getIsAuthenticated() const;
    std::string getHostName() const;
    std::list<Cmd> getCommands() const;

    // Setters
    void setFd(int fd);
    void setNickname(const std::string& nickname);
    void setUsername(const std::string& username);
    void setIPadd(const std::string& IPadd);
    void setIsAuthenticated(bool isAuthenticated);
    void setHostName(std::string hostName);
    void setCommands(std::list<Cmd> commands);

    // Other
    bool isOperator(const std::string& channel) const;
    void joinChannel(const std::string& channel);
    void leaveChannel(const std::string& channel);
};

#endif
