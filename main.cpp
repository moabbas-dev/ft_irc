#include <iostream>
#include <csignal>
#include <cstdlib> // For std::atoi
#include "includes/Server.hpp"
#include "includes/Cmd.hpp"

bool passwordValid(std::string pass) {
    for (size_t i = 0;i < pass.length(); i++) {
        if (!isalnum(pass.at(i)))
            return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " [port] [password [a-z][A-Z][0-9]]" << std::endl;
        return 1;
    }
    int port = std::atoi(argv[1]);
    if (port <= 0 || port > 65535) {
        std::cerr << "Error: Invalid port number. Port must be between 1 and 65535." << std::endl;
        return 1;
    }
    std::string password = std::string(argv[2]);
    if (!passwordValid(password)) {
        std::cerr << "Error: password must contain alphabetical characters or numbers only." << std::endl;
        return 1;   
    }

    Server server;
    std::cout << "---- SERVER ----" << std::endl;

    try {
        signal(SIGINT, Server::signalHandler);
        signal(SIGQUIT, Server::signalHandler);
        server.serverInit(port, password);
        server.run();
    }
    catch (const std::exception &e) {
        server.closeFds();
        std::cerr << "Error: " << e.what() << std::endl;
    }
    std::cout << "The Server Closed!" << std::endl;
    return 0;
}
