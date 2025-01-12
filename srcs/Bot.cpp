#include "../includes/Bot.hpp"
#include "../includes/Client.hpp"

Bot::Bot() : Client()
{
	this->nickname = "marvin";
	this->username = "bot";
	this->realname = "Marvin the bot";
	this->setHasSetNickName(true);
}

Bot::~Bot() { }

void Bot::connectToServer(Server &server)
{
    struct sockaddr_in serverAddr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

	std::string serverAddress = "127.0.0.1";
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server.getPort());
    serverAddr.sin_addr.s_addr = inet_addr(serverAddress.c_str());
	if (serverAddr.sin_addr.s_addr == INADDR_NONE) {
    	std::cerr << "Invalid address or address not supported" << std::endl;
    	return;
	}

    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return;
    }

    this->setFd(sockfd);
    std::cout << "Bot connected to server " << serverAddress << " on port " << server.getPort() << " with fd : " << sockfd << std::endl;

	// std::vector<std::string> passVec;
	// std::vector<std::string> nickVec;
	// std::vector<std::string> userVec;

	// passVec.push_back(server.getPassword());
	// // nickVec.push_back(nickname);
	// userVec.push_back(username);
	// userVec.push_back("0");
	// userVec.push_back("*");
	// userVec.push_back(realname);

	// Cmd passCmd("PASS", passVec);
	// // Cmd nickCmd("NICK", nickVec);
	// Cmd userCmd("USER", userVec);

	// commands.push_back(passCmd);
	// // commands.push_back(nickCmd);
	// commands.push_back(userCmd);

	// passCmd.execute(server, *this);
	// // nickCmd.execute(server, *this);
    // // std::cout << "here" << std::endl;
	// // Ktiiir 8aribee!! ana shlt l nickname am nfakket l segfault!!
	// userCmd.execute(server, *this);
}

void Bot::sendAgeMsg(Client& client) {
	(void)client;
}

void Bot::sendJokeMsg(Client& client) {
	(void)client;
}

void Bot::sendHelpMsg(Client& client) {
	std::string help = "Available commands are: !age(get your age) and !joke(get a joke)";
	std::string msg = ":" + this->getNickname() + "!~"
		+ this->getUsername() + "@localhost PRIVMSG "
		+ client.getNickname() + " :" + help + "\r\n";
	send(client.getFd(), msg.c_str(), msg.size(), 0);
}

void Bot::sendUnknowCmdMsg(Client& client) {
	(void)client;
}

// msg = ":" + bot.getNickname() + "!~"
// 		+ bot.getUsername() + "@localhost PRIVMSG "
// 		+ client.getNickname() + " :" + cmd.getParams()[1] + "\r\n";
// 	send(client.getFd(), msg.c_str(), msg.size(), 0);
