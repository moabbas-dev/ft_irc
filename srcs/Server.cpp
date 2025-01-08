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

const std::map<int, Client> &Server::getClients() const
{
    return this->clients;
}

std::string Server::getPassword() const {
    return password;
}

void Server::serverInit(int port, std::string password) {
    this->port = port;
    this->password = password;
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

    std::ostringstream oss;
    oss << newClient.getoriginalhostname() << ":" << newClient.getIPadd()
        << "<" << newClient.getFd() << ">" << " has connected.";
    Server::printResponse(oss.str(), GREEN);
}

void Server::signalHandler(int signum) {
    (void)signum;
    Server::isSignalReceived = true;
    std::cout << std::endl;
}

void Server::closeFds() {
    for(std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it) {
        std::ostringstream oss;
        std::string clientName = it->second.getHasSetNickName()? it->second.getNickname() : it->second.getoriginalhostname();
        oss << clientName << "<" << it->second.getFd() << "> disconnected.";
        Server::printResponse(oss.str(), RED);
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

void Server::removeClientFromChannels(int fd) {
    for (std::map<std::string, Channel>::iterator channel = channels.begin(); channel != channels.end();){
        Channel& currentChannel = channel->second;
		int clientDeleted = 0;
		if (currentChannel.isClientInChannel(fd)) {
            currentChannel.removeClient(fd);
            clientDeleted = 1;
        }
		else if (currentChannel.isOperator(fd)) {
            currentChannel.removeOperator(fd);
            clientDeleted = 1;
        }
		if (currentChannel.getClients().size() == 0) {
            std::map<std::string, Channel>::iterator next = channel;
            next++;
            channels.erase(channel);
            channel = next;
	        continue;
        }
		if (clientDeleted){
            std::map<int, Client>::const_iterator client = getClients().find(fd);
            if (client != getClients().end()) {
                std::string rpl = ":" + client->second.getNickname() + "!~" + client->second.getUsername() + "@localhost QUIT Quit\r\n";
                currentChannel.broadcastMessage(rpl, -1);
            }
		}
        ++channel;
	}
}

void Server::receiveData(int fd) {
    char buffer[1024] = {};

    ssize_t bytesRead = read(fd ,buffer ,sizeof(buffer) - 1);
    std::cout << buffer;
    std::map<int, Client>::iterator it = clients.find(fd);
    if(bytesRead > 0) {
        if (it != clients.end()) {
            Client& client = it->second;
            std::list<Cmd> commands;
            Parser::parse(&commands, std::string(buffer), client, *this);
            client.setCommands(commands);
        } else {
            std::cerr << "Error: Client not found for FD " << fd << std::endl;
        }
    }
    else if(bytesRead == 0) {
        if (it != clients.end()) {
            std::ostringstream oss;
            std::string clientName = it->second.getHasSetNickName() ? it->second.getNickname() : it->second.getoriginalhostname();
            oss << clientName << "<" << it->second.getFd() << "> disconnected.";
            Server::printResponse(oss.str(), RED);
        }
        (removeClientFromChannels(fd), clearClient(fd), close(fd));
    } else {
        std::cerr << "Error reading from client\n";
        (removeClientFromChannels(fd), clearClient(fd), close(fd));
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
                          << client.getHostName() << "<" << client.getFd() << "> "
                          << ": " << e.what() << std::endl;
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

void Server::printResponse(const std::string& message, const char *color) {
    time_t now = time(0);
    struct tm* timeinfo = localtime(&now);

    char dateBuffer[20];
    strftime(dateBuffer, sizeof(dateBuffer), "%Y-%m-%d, %H:%M", timeinfo);

    std::cout << color << "[" << dateBuffer << "]: " << RESET << message;
    if (!message.empty())
        std::cout << std::endl;
}

std::map<std::string, Channel>& Server::getChannels()  {
    return channels;
}


Channel* Server::findChannel(const std::string& channel_name) {
    std::map<std::string, Channel>::iterator it = channels.find(channel_name);
    if (it != channels.end()) {
        return &(it->second);
    }
    return NULL;
}

void Server::deleteChannel(const std::string& channel_name) {
    channels.erase(channel_name);
}

void Server::setChannels(std::map<std::string, Channel> channels) {
    this->channels = channels;
}

void Server::sendReply(std::string mesgArgs[], int fd, messageCode messageCode) {
    std::ostringstream result;

    switch (messageCode)
    {
    case RPL_WELCOME:
        result << RPL_CONNECTED(mesgArgs[0]);
        break;
    case RPL_CHANNELMODEIS:
        result << RPL_CHANNELMODEIS(mesgArgs[0], mesgArgs[1], mesgArgs[2]);
        break;
    case RPL_CREATIONTIME:
        result << RPL_CREATIONTIME(mesgArgs[0], mesgArgs[1], mesgArgs[2]);
        break;
    case RPL_TOPIC:
        result << RPL_TOPICIS(mesgArgs[0], mesgArgs[1], mesgArgs[2]);
        break;
    case RPL_NOTOPIC:
        result << RPL_NOTOPIC(mesgArgs[0], mesgArgs[1]);
        break;
    case RPL_TOPICWHOTIME:
        result << RPL_TOPICWHOTIME(mesgArgs[0], mesgArgs[1], mesgArgs[2], mesgArgs[3]);
        break;
    case RPL_NAMREPLY:
        result << RPL_NAMREPLY(mesgArgs[0], mesgArgs[1], mesgArgs[2]);
        break;
    case RPL_ENDOFNAMES:
        result << RPL_ENDOFNAMES(mesgArgs[0], mesgArgs[1]);
        break;
    case RPL_CREATECHANNELMSG:
        result << RPL_JOINMSG(mesgArgs[0], mesgArgs[1], mesgArgs[2])
               << RPL_NAMREPLY(mesgArgs[3], mesgArgs[2], mesgArgs[4])
               << RPL_ENDOFNAMES(mesgArgs[3], mesgArgs[2]);
        break;
    case RPL_NICKCHANGE:
        result << RPL_NICKCHANGE(mesgArgs[0], mesgArgs[1]);
        break;
    case RPL_JOINMSG:
        result << RPL_JOINMSG(mesgArgs[0], mesgArgs[1], mesgArgs[2]);
        break;
    case RPL_UMODEIS:
        result << RPL_UMODEIS(mesgArgs[0], mesgArgs[1], mesgArgs[2], mesgArgs[3]);
        break;
    case RPL_CHANGEMODE:
        result << RPL_CHANGEMODE(mesgArgs[0], mesgArgs[1], mesgArgs[2], mesgArgs[3]);
        break;
    case RPL_TOPICADMIN:
        result << RPL_TOPICADMIN(mesgArgs[0], mesgArgs[1], mesgArgs[2], mesgArgs[3]);
        break;
    case RPL_NICKNAMECHANGED:
        result << RPL_NICKCHANGE(mesgArgs[0], mesgArgs[1]);
        break;
    case ERR_BADCHANNELMASK:
        result << ERR_BADCHANNELMASK(mesgArgs[0], mesgArgs[1]);
        break;
    case RPL_INVITING:
        result << RPL_INVITING(mesgArgs[0], mesgArgs[1], mesgArgs[2]);
        break;
    case RPL_KICK:
        result << RPL_KICK(mesgArgs[0], mesgArgs[1], mesgArgs[2], mesgArgs[3], mesgArgs[4]);
        break;
    case RPL_AWAY:
        result << RPL_AWAY(mesgArgs[0], mesgArgs[1], mesgArgs[2]);
        break;
    default:
        break;
    }
    if (send(fd, result.str().c_str(), result.str().size(), 0) == -1)
        std::cerr << "Cannot Send reply to fd=" << fd << std::endl;
}

bool Server::channelExistInServer(const std::string &channelName)
{
    for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->first == channelName)
            return true;
    }
    return false;
}

bool Server::clientIsInServer(const std::string &nickname)
{
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second.getNickname() == nickname)
			return true;
	}
	return false;
}

bool Server::isAChannelName(const std::string &str)
{
    std::map<std::string, Channel>::iterator it = channels.begin();
    while (it != channels.end())
    {
        if (it->first == str)
            return true;
        ++it;
    }
    return false;
}

bool Server::isAClientName(const std::string &str)
{
    std::map<int, Client>::iterator it = clients.begin();
    while (it != clients.end())
    {
        if (it->second.getNickname() == str)
            return true;
    }
    return false;
}

Channel *Server::getSpecifiedChannel(const std::string &channelName)
{
    if (channels.empty())
        return NULL;
    for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if (it->first == channelName)
            return &(it->second);
    }
    return NULL;
}

Client *Server::getSpecifiedClient(const std::string &nickName)
{
    if (clients.empty())
        return NULL;
    for (std::map<int, Client>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->second.getNickname() == nickName)
            return &(it->second);
    }
    return NULL;
}

void Server::sendError(std::string mesgArgs[], int fd, messageCode messageCode) {
    std::ostringstream result;

    switch (messageCode)
    {
    case ERR_NOSUCHNICK:
        result << ERR_NOSUCHNICK(mesgArgs[0], mesgArgs[1]);
        break;
    case ERR_NOSUCHCHANNEL:
        result << ERR_NOSUCHCHANNEL(mesgArgs[0], mesgArgs[1]);
        break;
    case ERR_UNKNOWCOMMAND:
        result << ERR_CMDNOTFOUND(mesgArgs[0], mesgArgs[1]);
        break;
    case ERR_NONICKNAMEGIVEN:
        result << ERR_NONICKNAMEGIVEN(mesgArgs[0]);
        break;
    case ERR_ERRONEUSNICK:
        result << ERR_ERRONEUSNICK(mesgArgs[0]);
        break;
    case ERR_NICKINUSE:
        result << ERR_NICKINUSE(mesgArgs[0]);
        break;
    case ERR_NOTREGISTERED:
        result << ERR_NOTREGISTERED(mesgArgs[0]);
        break;
    case ERR_NOTENOUGHPARAM:
        result << ERR_NOTENOUGHPARAM(mesgArgs[0]);
        break;
    case ERR_ALREADYREGISTERED:
        result << ERR_ALREADYREGISTERED(mesgArgs[0]);
        break;
    case ERR_PASSWDMISMATCH:
        result << ERR_PASSWDMISMATCH(mesgArgs[0]);
        break;
    case ERR_KEYSET:
        result << ERR_KEYSET(mesgArgs[0]);
        break;
    case ERR_UNKNOWNMODE:
        result << ERR_UNKNOWNMODE(mesgArgs[0], mesgArgs[1], mesgArgs[2]);
        break;
    case ERR_CHANOPRIVSNEEDED:
        result << ERR_NOTOPERATOR(mesgArgs[0], mesgArgs[1]);
        break;
    case ERR_NEEDMODEPARM:
        result << ERR_NEEDMODEPARM(mesgArgs[0], mesgArgs[1]);
        break;
    case ERR_INVALIDMODEPARM:
        result << ERR_INVALIDMODEPARM(mesgArgs[0], mesgArgs[1]);
        break;
    case ERR_USERONCHANNEL:
        result << ERR_USERONCHANNEL(mesgArgs[0], mesgArgs[1]);
        break;
    case ERR_NOTONCHANNEL:
        result << ERR_NOTONCHANNEL(mesgArgs[0], mesgArgs[2]);
        break;
    case ERR_BADCHANNELKEY:
        result << ERR_BADCHANNELKEY(mesgArgs[0], mesgArgs[1]);
        break;
    case ERR_BADCHANNELMASK:
        result << ERR_BADCHANNELMASK(mesgArgs[0], mesgArgs[1]);
        break;
    case ERR_CHANNELISFULL:
        result << ERR_CHANNELISFULL(mesgArgs[0], mesgArgs[1]);
        break;
    case ERR_INVITEONLYCHAN:
        result << ERR_INVITEONLYCHAN(mesgArgs[0], mesgArgs[1]);
        break;
    case ERR_TOOMANYCHANNELS:
        result << ERR_TOOMANYCHANNELS(mesgArgs[0], mesgArgs[1]);
        break;
    case ERR_NOTONTHATCHANNEL:
        result << ERR_NOTONTHATCHANNEL(mesgArgs[0], mesgArgs[1]);
        break;
    case ERR_NOTEXTTOSEND:
        result << ERR_NOTEXTTOSEND(mesgArgs[0]);
        break;
    case ERR_CANNOTSENDTOCHAN:
        result << ERR_CANNOTSENDTOCHAN(mesgArgs[0], mesgArgs[1]);
        break;
    default:
        break;
    }

    if (send(fd, result.str().c_str(), result.str().size(), 0) == -1)
        std::cerr << "Cannot Send error to fd=" << fd << std::endl;
}

