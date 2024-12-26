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

class Server {
private:
    int port;
    int serSocketFd;
    static bool isSignalReceived;
    std::vector<Client> clients;
    std::vector<struct pollfd> fds;
    std::string password;
    std::map<std::string, Channel> channels;

public:
    Server();
    Server(int port, const std::string& password);
    void serverInit(int port);
    void createServerSocket();
    void acceptNewClient();
    void receiveData(int fd);
    static void signalHandler(int signum);
    void closeFds();
    void clearClients(int fd);
    void run();
    void handleJoin(int fd, const std::string& channelName);
    void handlePrivmsg(int senderFd, const std::string& target, const std::string& message);
    void handlePart(int fd, const std::string& channelName);
};

#endif
