#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include "Cmd.hpp"
#include "Server.hpp"
class Cmd;
class Channel;
class Server;
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
    std::string messageBuffer;
    std::string hostName;
    // these channels are copies from the server channels
    std::vector<Channel> channels;

    // temporary channels that are syntaxly true but we dont know if are exists or not
    std::vector<Channel> tmp_channels;

    //store the commands for each client here (valid commands only) to execute them
    std::list<Cmd> commands; 

    // store the invitations received by the client
    std::map<std::string, bool> invitationsBox;

public:
    Client();
    ~Client();

    // Getters
    int getFd() const;
    std::string getNickname() const;
    std::string getUsername() const;
    std::string getRealname() const;
    std::string getIPadd() const;
    std::string getHostName() const;
    std::string getoriginalhostname() const;
    bool getIsAuthenticated() const;
    bool getHasSetPassword() const;
    bool getHasSetNickName() const;
    bool getHasSetUser() const;
    std::list<Cmd> getCommands() const;
    std::vector<Channel>& getChannels() ;
    std::vector<Channel> getTempChannels() const;
    std::map<std::string, bool> &getInvitationsBox();

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
    void setTempChannels(std::vector<Channel> tmp_channels);

    // Other
    bool isOperator(Channel& channel) const;
    void clearTempChannels() ;
    bool isInsideTheChannel(const std::string &channelName);
    void removeChannel(Channel& channel);
    bool isOperatorInChannel(const std::string &channelName, Server &server);
    void addInvitationToChannel(const std::string &channelName);
    void removeChannelInvitation(const std::string &channelName);
    bool isInvitedTochannel(Channel& channel);
};

#endif