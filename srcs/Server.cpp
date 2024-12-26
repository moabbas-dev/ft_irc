#include "../includes/Server.hpp"

bool Server::isSignalReceived = false;

Server::Server() {

}

Server::Server(int port, std::string password) {
    this->port = port;
    this->password = password;
}

void Server::serverInit() {
	this->port = 4444;
	createServerSocket();

	std::cout << "Server <" << serSocketFd << " > Connected\n";
	std::cout << "Waiting to accept a connection from a client ...\n";
}

void Server::createServerSocket() {
	struct sockaddr_in add;
	add.sin_family = AF_INET; //-> set the address family to ipv4
	add.sin_port = htons(this->port); //-> convert the port to network byte order (big endian)
	add.sin_addr.s_addr = INADDR_ANY; //-> set the address to any local machine address

	// int socket(int domain, int type, int protocol);
	serSocketFd = socket(AF_INET, SOCK_STREAM, 0); // AF_INET for IPv4 communication and AF_INET6 for IPv6 communication, SOCK_STREAM for TCP sockets, SOCK_DGRAM for UDP sockets
	if (serSocketFd < 0)
		throw(std::runtime_error("Error: failed to create socket"));

	int en = 1;// Setting the en value to 1 indicates that the option is enabled.
	if (setsockopt(serSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) //-> set the socket option (SO_REUSEADDR) to reuse the address
		throw(std::runtime_error("Error: failed to set option (SO_REUSEADDR) on socket"));
	// F_SETFL, indicating that you want to set the file status flags.
	if (fcntl(serSocketFd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw std::runtime_error("Error: faild to set option (O_NONBLOCK) on socket");
	if (bind(serSocketFd, (struct sockaddr* )&add, sizeof(add)) == -1) //-> bind the socket to the address
		throw std::runtime_error("Error: faild to bind socket");
	if (listen(serSocketFd, SOMAXCONN) == -1) //-> listen for incoming connections and making the socket a passive socket
		throw std::runtime_error("Error: listen for incoming connections failed");

	struct pollfd newPoll;
	newPoll.fd = serSocketFd;
	newPoll.events = POLLIN; // POLLIN:(any readable data available) 
	newPoll.revents = 0;
	fds.push_back(newPoll); //-> add the server socket to the poll list
}

void Server::acceptNewClient() {
	struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    //-> Accept the client connection
    int clientFd = accept(serSocketFd, (struct sockaddr *)&clientAddr, &addrLen);
    if (clientFd < 0) {
		//-> Ignore EWOULDBLOCK since we're in non-blocking mode
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

    //-> Create and set up the new client
    Client newClient;
    newClient.setFd(clientFd);
	//-> inet_ntoa: convert ip address to char*
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

void Server::recieveData(int fd) {
    char buffer[1024]; // Buffer to store received data
    memset(buffer, 0, sizeof(buffer)); // Clear the buffer

    // Read data from the client
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer) - 1); // Leave space for null terminator
    if (bytesRead > 0) {
        // Data received successfully
        std::cout << "Received from client <" << fd << ">: " << buffer << std::endl;

        // Process data (for now, just echo it back)
        std::string response = "Server received: " + std::string(buffer);
        if (write(fd, response.c_str(), response.size()) == -1) {
            perror("Error sending response to client");
        }
    } else if (bytesRead == 0) {
        // Client disconnected
        std::cout << "Client <" << fd << "> disconnected." << std::endl;
        close(fd); // Close the client socket
        clearClients(fd); // Remove client from the poll list and clients vector
    } else {
        // Error occurred
        perror("Error reading from client");
        close(fd); // Close the client socket
        clearClients(fd); // Clean up resources
    }
}

void Server::signalHandler(int signum) {
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::isSignalReceived = true;
}

void Server::closeFds() {
	for (size_t i = 0;i < clients.size();i++) {
		std::cout << "Client <" << clients[i].getFd() << "> Disconnected\n";
		close(clients[i].getFd());
	}
}

void Server::clearClients(int fd) {
	for (size_t i = 0;i < fds.size();i++) 
		if (fds[i].fd == fd) {
			fds.erase(fds.begin() + i);
			break;
		}

	for (size_t i = 0;i < clients.size();i++)
		if (clients[i].getFd() == fd) {
			clients.erase(clients.begin() + i);
			break;
		}
}

void Server::run() {
    while (!isSignalReceived) {
        // Wait for events using poll
        int pollCount = poll(fds.data(), fds.size(), -1); // -1 for infinite timeout
        if (pollCount == -1) {
            if (errno == EINTR) continue; // Interrupted by signal, retry
            perror("poll failed");
            break;
        }

        // Iterate through the poll list to handle events
        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == serSocketFd) {
                    // Server socket is readable -> Accept new client
                    acceptNewClient();
                } else {
                    // Client socket is readable -> Receive data
                    recieveData(fds[i].fd);
                }
            }
        }
    }

    // Cleanup before exiting
    closeFds();
    std::cout << "Server shutting down gracefully." << std::endl;
}

void Server::handleJoin(int fd, const std::string& channelName) {
    // Locate or create the channel
    Channel* channel = NULL;
    std::map<std::string, Channel>::iterator it = channels.find(channelName);
    if (it == channels.end()) {
        // Channel does not exist, create it
        channels[channelName] = Channel(channelName);
        channel = &channels[channelName];
    } else {
        channel = &it->second;
    }

    // Retrieve the client
    std::vector<Client>::iterator clientIt = std::__find_if(clients.begin(), clients.end(), [fd](const Client& c) {
        return c.getFd() == fd;
    });

    if (clientIt == clients.end()) {
        send(fd, "Error: Client not found.\r\n", 26, 0);
        return;
    }

    // Add the client to the channel
    Client& client = *clientIt;
    if (!channel->addClient(client)) {
        send(fd, "Error: Could not join channel. Check the key or user limit.\r\n", 61, 0);
        return;
    }

    // Notify the client
    std::string joinedChannelMsg = "Joined channel " + channelName + "\r\n";
    send(fd, joinedChannelMsg.c_str(), joinedChannelMsg.size(), 0);

    // Notify other members in the channel
    std::string joinMsg = client.getNickname() + " has joined the channel.\r\n";
    channel->broadcastMessage(joinMsg, fd);
}
