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
    void serverInit(int port, std::string password);
    void createServerSocket();
    void acceptNewClient();
    void receiveData(int fd);
    static void signalHandler(int signum);
    void closeFds();
    void clearClient(int fd);
    void run();
    std::string getPassword() const;
    void handleJoin(int fd, const std::string& channelName);
    void handlePrivmsg(int senderFd, const std::string& target, const std::string& message);
    void handlePart(int fd, const std::string& channelName);
private:
    void checkInitialClientData(int fd);
    void processClientCommands(int fd);
};

#endif