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
    std::string realname;
    bool isAuthenticated;
    bool hasSetPassword;
    bool hasSetNickName;
    bool hasSetUser;
    std::vector<Channel> channels;
    std::string messageBuffer;
    std::string hostName;
    std::list<Cmd> commands;

public:
    Client();
    // Getters
    int getFd() const;
    std::string getNickname() const;
    std::string getUsername() const;
    std::string getRealname() const;
    std::string getIPadd() const;
    bool getIsAuthenticated() const;
    std::string getHostName() const;
    std::list<Cmd> getCommands() const;
    bool getHasSetPassword() const;
    bool getHasSetNickName() const;
    bool getHasSetUser() const;
    std::vector<Channel> getChannels() const;

    // Setters
    void setFd(int fd);
    void setNickname(const std::string& nickname);
    void setUsername(const std::string& username);
    void setRealname(const std::string& realname);
    void setIPadd(const std::string& IPadd);
    void setIsAuthenticated(bool isAuthenticated);
    void setHostName(std::string hostName);
    void setCommands(std::list<Cmd> commands);
    void setHasSetPassword(bool hasSetPassword);
    void setHasSetNickName(bool hasSetNickName);
    void setHasSetUser(bool hasSetUser);
    void setChannels(std::vector<Channel> channels);

    // Other
    bool isOperator(Channel& channel) const;
    void joinChannel(const std::string& channel);
    void leaveChannel(const std::string& channel);
};

#endif
