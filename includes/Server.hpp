#ifndef SERVER_HPP
#define SERVER_HPP

#include <vector>
#include <map>
#include <string>
#include "Client.hpp"
#include "Channel.hpp"
#include <poll.h>
#include <unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>
#include <arpa/inet.h> 
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <stdexcept>
#include <iostream>
#include <list>
#include <netdb.h>
#include "Replies.hpp"
#include "Cmd.hpp"
#include "Bot.hpp"

#define RESET "\033[0m"
#define BLUE "\033[34m"
#define GREEN "\033[32m"
#define RED "\033[31m"

class Client;
class Channel;

class Server {
private:
    int port;
    int serSocketFd;
    static bool isSignalReceived;
    std::map<int, Client> clients;
    std::vector<struct pollfd> fds;
    std::string password;
    std::map<std::string, Channel> channels;
    std::map<int, std::string> clientBuffers;

public:
    Server();
    Server(int port, const std::string& password);
    ~Server();
    const std::map<int, Client> &getClients() const;
    std::map<int, Client> &getClients() ;
    void serverInit(int port, std::string password);
    void createServerSocket();
    void acceptNewClient();
    void receiveData(int fd);
    static void signalHandler(int signum);
    void closeFds();
    void clearClient(int fd);
    void removeClientFromChannels(int fd);
    void run();
    int getPort() const;
    std::string getPassword() const;
    std::map<std::string, Channel>& getChannels() ;
    void setChannels(std::map<std::string, Channel> channels);
    Channel *findChannel(const std::string& channel_name);
    void deleteChannel(const std::string& channel_name);
    bool channelExistInServer(const std::string &channelName);
    bool clientIsInServer(const std::string &nickname);
    bool isAClientName(const std::string &str);
    bool isAChannelName(const std::string &str);
    Channel *getSpecifiedChannel(const std::string &channelName);
    Client *getSpecifiedClient(const std::string &nickName);

    static void printResponse(const std::string& message, const char* color);
    static void sendReply(std::string mesgArgs[], int fd, messageCode messageCode);
    static void sendError(std::string mesgArgs[], int fd, messageCode messageCode);

private:
    void processClientCommands(int fd);
};

#endif