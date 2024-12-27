#include <iostream>
#include <csignal>
#include <cstdlib> // For std::atoi
#include "includes/Server.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " [port]" << std::endl;
        return 1;
    }
    int port = std::atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        std::cerr << "Error: Invalid port number. Port must be between 1 and 65535." << std::endl;
        return 1;
    }

    Server server;
    std::cout << "---- SERVER ----" << std::endl;

    try {
        signal(SIGINT, Server::signalHandler);
        signal(SIGQUIT, Server::signalHandler);
        server.serverInit(port);
        server.run();
    }
    catch (const std::exception &e) {
        server.closeFds();
        std::cerr << "Error: " << e.what() << std::endl;
    }

    std::cout << "The Server Closed!" << std::endl;
    return 0;
}
