#include "../includes/Server.hpp"

bool Server::isSignalReceived = false;

Server::Server() : port(0), serSocketFd(-1) {}

Server::Server(int port, const std::string& password)
    : port(port), serSocketFd(-1), password(password) {}

void Server::serverInit(int port) {
    this->port = port;
    createServerSocket();
    std::cout << "Server <" << serSocketFd << "> Connected\n";
    std::cout << "Waiting to accept a connection from a client...\n";
}

void Server::createServerSocket() {
    struct sockaddr_in add;
    add.sin_family = AF_INET;
    add.sin_port = htons(this->port);
    add.sin_addr.s_addr = INADDR_ANY;

    serSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serSocketFd < 0)
        throw std::runtime_error("Error: failed to create socket");

    int en = 1;
    if (setsockopt(serSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
        throw std::runtime_error("Error: failed to set option (SO_REUSEADDR) on socket");

    if (fcntl(serSocketFd, F_SETFL, O_NONBLOCK) == -1)
        throw std::runtime_error("Error: failed to set option (O_NONBLOCK) on socket");

    if (bind(serSocketFd, (struct sockaddr*)&add, sizeof(add)) == -1)
        throw std::runtime_error("Error: failed to bind socket");

    if (listen(serSocketFd, SOMAXCONN) == -1)
        throw std::runtime_error("Error: failed to listen for incoming connections");

    struct pollfd newPoll;
    newPoll.fd = serSocketFd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;
    fds.push_back(newPoll);
}

void Server::acceptNewClient() {
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    int clientFd = accept(serSocketFd, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientFd < 0) {
        if (errno != EWOULDBLOCK) {
            perror("Error: accept client connection failed");
        }
        return;
    }

    if (fcntl(clientFd, F_SETFL, O_NONBLOCK) == -1) {
        perror("Error: failed to set non-blocking mode on client socket");
        close(clientFd);
        return;
    }

    Client newClient;
    newClient.setFd(clientFd);
    newClient.setIPadd(std::string(inet_ntoa(clientAddr.sin_addr)));
    clients.push_back(newClient);

    struct pollfd newPoll;
    newPoll.fd = clientFd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;
    fds.push_back(newPoll);

    std::cout << "New client connected: FD = " << clientFd
              << ", IP = " << newClient.getIPadd() << std::endl;
}

void Server::receiveData(int fd) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
    if (bytesRead > 0) {
        std::cout << "Received from client <" << fd << ">: " << buffer << std::endl;
        std::string response = "Server received: " + std::string(buffer);
        if (write(fd, response.c_str(), response.size()) == -1) {
            perror("Error sending response to client");
        }
    } else if (bytesRead == 0) {
        std::cout << "Client <" << fd << "> disconnected." << std::endl;
        close(fd);
        clearClients(fd);
    } else {
        perror("Error reading from client");
        close(fd);
        clearClients(fd);
    }
}

void Server::signalHandler(int signum) {
    (void)signum;
    std::cout << std::endl << "Signal Received!" << std::endl;
    Server::isSignalReceived = true;
}

void Server::closeFds() {
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        std::cout << "Client <" << it->getFd() << "> Disconnected\n";
        close(it->getFd());
    }
}

void Server::clearClients(int fd) {
    for (std::vector<struct pollfd>::iterator it = fds.begin(); it != fds.end(); ++it) {
        if (it->fd == fd) {
            fds.erase(it);
            break;
        }
    }
    for (std::vector<Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        if (it->getFd() == fd) {
            clients.erase(it);
            break;
        }
    }
}

void Server::run() {
    while (!isSignalReceived) {
        int pollCount = poll(&fds[0], fds.size(), -1);
        if (pollCount == -1) {
            if (errno == EINTR) continue;
            perror("poll failed");
            break;
        }

        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == serSocketFd) {
                    acceptNewClient();
                } else {
                    receiveData(fds[i].fd);
                }
            }
        }
    }

    closeFds();
    std::cout << "Server shutting down gracefully." << std::endl;
}
