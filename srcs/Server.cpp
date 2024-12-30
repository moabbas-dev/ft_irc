#include "../includes/Server.hpp"
#include "../includes/Cmd.hpp"
#include "../includes/Client.hpp"
#include "../includes/Channel.hpp"

bool Server::isSignalReceived = false;

Server::Server(): port(0) ,serSocketFd(-1) {}

Server::Server(int port ,const std::string& password)
    : port(port) ,serSocketFd(-1) ,password(password) {}

Server::~Server() {
    for (std::map<int, Client>::iterator it = clients.begin();it != clients.end(); ++it)
        it->second.getCommands().clear();
}

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

    serSocketFd = socket(AF_INET ,SOCK_STREAM ,0);
    if(serSocketFd < 0)
        throw std::runtime_error("Error: failed to create socket");

    int en = 1;
    if(setsockopt(serSocketFd ,SOL_SOCKET ,SO_REUSEADDR ,&en ,sizeof(en)) == -1)
        throw std::runtime_error("Error: failed to set option (SO_REUSEADDR) on socket");

    if(fcntl(serSocketFd ,F_SETFL ,O_NONBLOCK) == -1)
        throw std::runtime_error("Error: failed to set option (O_NONBLOCK) on socket");

    if(bind(serSocketFd ,(struct sockaddr*)&add ,sizeof(add)) == -1)
        throw std::runtime_error("Error: failed to bind socket");

    if(listen(serSocketFd ,SOMAXCONN) == -1)
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

    int clientFd = accept(serSocketFd ,(struct sockaddr*)&clientAddr ,&addrLen);
    if(clientFd < 0) {
        std::cerr << "Error: accept client connection failed\n";
        return;
    }

    if(fcntl(clientFd ,F_SETFL ,O_NONBLOCK) == -1) {
        std::cerr << "Error: failed to set non-blocking mode on client socket\n";
        close(clientFd);
        return;
    }


    std::string ipAddress = std::string(inet_ntoa(clientAddr.sin_addr));
    std::string hostname = "Unknown";
    struct hostent* host = gethostbyaddr(&clientAddr.sin_addr, sizeof(clientAddr.sin_addr), AF_INET);
    if (host && host->h_name)
        hostname = std::string(host->h_name);
    Client newClient;
    newClient.setFd(clientFd);
    newClient.setIPadd(std::string(inet_ntoa(clientAddr.sin_addr)));
    newClient.setHostName(hostname);
    clients.insert(std::pair<int, Client>(clientFd, newClient));

    struct pollfd newPoll;
    newPoll.fd = clientFd;
    newPoll.events = POLLIN;
    newPoll.revents = 0;
    fds.push_back(newPoll);

    std::cout << "[New Client]: " << newClient.getHostName() << ":" << newClient.getIPadd() << " has connected.\n";
}

// void Server::receiveDataV1(int fd) {
//     char buffer[1024];
//     memset(buffer, 0, sizeof(buffer));

//     ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
//     if (bytesRead > 0) {
//         std::cout << "Received from client <" << fd << ">: " << buffer << std::endl;
//         std::string response = "Server received: " + std::string(buffer);
//         if (write(fd, response.c_str(), response.size()) == -1)
//             std::cerr << "Error sending response to client\n";
//     } else if (bytesRead == 0) {
//         std::cout << "Client <" << fd << "> disconnected." << std::endl;
//         close(fd);
//         clearClients(fd);
//     } else {
//         std::cerr<< "Error reading from client\n";
//         close(fd);
//         clearClients(fd);
//     }
// }



// void Server::receiveDataV2(int fd) {
//     char buffer[1024];
//     memset(buffer, 0, sizeof(buffer));

//     ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1);
//     if (bytesRead > 0) {
//         std::string input(buffer);
//         std::string trimmed;
//         std::string commandName;
//         std::vector<std::string> params;

//         // Parse the command
//         trimmed = trimString(input);
//         if (trimmed == "" || trimmed == "CAP LS")
//             return;
//         size_t pos = trimmed.find(' ');
//         if (pos != std::string::npos) {
//             commandName = trimmed.substr(0, pos);
//             std::string paramString = trimmed.substr(pos + 1);

//             size_t start = 0;
//             size_t end;
//             while ((end = paramString.find(' ', start)) != std::string::npos) {
//                 params.push_back(paramString.substr(start, end - start));
//                 start = end + 1;
//             }
//             if (start < paramString.length())
//                 params.push_back(paramString.substr(start));
//         } else {
//             commandName = trimmed;
//         }

//         // Create and execute the command
//         Cmd cmd(commandName, params);
//         for (size_t i = 0; i < clients.size(); ++i) {
//             if (clients[i].getFd() == fd) {
//                 cmd.execute(*this, clients[i]);
//                 break;
//             }
//         }
//     } else if (bytesRead == 0) {
//         std::cout << "Client <" << fd << "> disconnected." << std::endl;
//         close(fd);
//         clearClients(fd);
//     } else {
//         std::cerr << "Error reading from client\n";
//         close(fd);
//         clearClients(fd);
//     }
// }

void Server::signalHandler(int signum) {
    (void)signum;
    std::cout << std::endl << "Signal Received!" << std::endl;
    Server::isSignalReceived = true;
}

void Server::closeFds() {
    for(std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        std::cout << "Client " << it->second.getHostName() << "<" << it->second.getFd() << "> disconnected." << std::endl;
        close(it->second.getFd());
    }
}

void Server::clearClient(int fd) {
    for(std::vector<struct pollfd>::iterator it = fds.begin(); it != fds.end(); ++it) {
        if(it->fd == fd) {
            fds.erase(it);
            break;
        }
    }
    clients.erase(fd);
}

void Server::receiveData(int fd) {
    char buffer[1024] = {};

    ssize_t bytesRead = read(fd ,buffer ,sizeof(buffer) - 1);
    std::map<int, Client>::iterator it = clients.find(fd);
    if(bytesRead > 0) {
        if (it != clients.end()) {
            Client& client = it->second;
            std::list<Cmd> commands;
            Parser::parse(&commands, std::string(buffer), fd);
            client.setCommands(commands);
        } else {
            std::cerr << "Error: Client not found for FD " << fd << std::endl;
        }
    }
    else if(bytesRead == 0) {
        std::cout << "Client " << it->second.getHostName() << "<" << fd << "> disconnected." << std::endl;
        close(fd);
        clearClient(fd);
    } else {
        std::cerr << "Error reading from client\n";
        close(fd);
        clearClient(fd);
    }
}

void Server::processClientCommands(int fd) {
    std::map<int, Client>::iterator it = clients.find(fd);
    if (it != clients.end()) {
        Client& client = it->second;
        std::list<Cmd> commands = client.getCommands();
        while (!commands.empty()) {
            Cmd command = commands.front();
            commands.pop_front();

            try {
                command.execute(*this, client);
            } catch (const std::exception& e) {
                std::cerr << "Error executing command for client "
                            << client.getFd() << ": " << e.what() << std::endl;
            }
        }
    }
}

void Server::run() {
    while(!isSignalReceived) {
        int pollCount = poll(&fds[0] ,fds.size() ,-1);
        if(pollCount == -1) {
            if (isSignalReceived)
                break;
            std::cerr << "Error: poll failed\n";
            break;
        }

        for(size_t i = 0; i < fds.size(); ++i) {
            if(fds[i].revents & POLLIN) {
                if(fds[i].fd == serSocketFd)
                    acceptNewClient();
                else {
                    receiveData(fds[i].fd);
                    processClientCommands(fds[i].fd);
                }
            }
        }
    }
    closeFds();
}
