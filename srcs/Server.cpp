#include "../includes/Server.hpp"

bool Server::isSignalReceived = false;

Server::Server() {

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
 (void) fd;
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
