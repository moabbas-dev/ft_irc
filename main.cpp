#include "./includes/Server.hpp"

int main()
{
	Server server;
	std::cout << "---- SERVER ----" << std::endl;
	try{
		signal(SIGINT, Server::signalHandler); //-> catch the signal (ctrl + c)
		signal(SIGQUIT, Server::signalHandler); //-> catch the signal (ctrl + \)
		server.serverInit(); //-> initialize the server
	}
	catch(const std::exception& e){
		server.closeFds(); //-> close the file descriptors
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
	return 0;
}
