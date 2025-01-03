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
    void serverInit(int port, std::string password);
    void createServerSocket();
    void acceptNewClient();
    void receiveData(int fd);
    static void signalHandler(int signum);
    void closeFds();
    void clearClient(int fd);
    void run();
    std::string getPassword() const;
    std::map<std::string, Channel>& getChannels() ;
    void setChannels(std::map<std::string, Channel> channels);
    Channel *findChannel(const std::string& channel_name);
    void deleteChannel(const std::string& channel_name);
    
    void handleJoin(int fd, const std::string& channelName);
    void handlePrivmsg(int senderFd, const std::string& target, const std::string& message);
    void handlePart(int fd, const std::string& channelName);
    static void printResponse(const std::string& message, const char* color);

private:
    void processClientCommands(int fd);
};

#endif