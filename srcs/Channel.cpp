#include "../includes/Channel.hpp"

Channel::Channel(std::string name, std::string key): name(name), channelKey(key) {
    mode['i'] = false; // Invite-only mode
    mode['t'] = false; // Topic restriction mode
    mode['k'] = false; // Password/key restriction mode
    mode['o'] = false; // Operator mode
    mode['l'] = false; // User limit mode
}

Channel::Channel(std::string name): name(name) {
    mode['i'] = false;
    mode['t'] = false;
    mode['k'] = false;
    mode['o'] = false;
    mode['l'] = false;
}

Channel::~Channel() { }

std::string Channel::getName() const {
	return name;
}

std::string Channel::getTopic() const {
	return topic;
}

void Channel::setTopic(const std::string& topic, int fd) {
	if (topicRestricted && !isOperator(fd)) {
        throw std::runtime_error("Permission denied: Only operators can set the topic.");
    }
    this->topic = topic;	
}

const std::string& Channel::getChannelKey() const {
    return channelKey;
}

void Channel::setChannelKey(const std::string& key) {
    channelKey = key;
}

bool Channel::isClientInChannel(int fd) const {
	for (size_t i = 0;i < clients.size();i++) {
        if (clients.at(i).getFd() == fd)
            return true;
    }
    return false;
}

bool Channel::isOperator(int fd) const {
    std::map<int, bool>::const_iterator it = operators.find(fd);
    return it != operators.end() && it->second;
}

bool Channel::addClient(Client& client, const std::string& key) {
    for (size_t i = 0;i < clients.size();i++) {
        if (clients[i].getFd() == client.getFd()) {
            return false; // already in the channel
        }
    }

    // If the channel has a key, verify it
    if (!channelKey.empty() && channelKey != key)
        return false; // Invalid key provided

    clients.push_back(client);
    return true;
}

void Channel::removeClient(int fd) {
    for (size_t i = 0; i < clients.size(); ++i) {
        if (clients[i].getFd() == fd) {
            clients.erase(clients.begin() + i);
            break;
        }
    }

    // Remove the client from the operators list if they are an operator
    std::map<int, bool>::iterator it = operators.find(fd);
    if (it != operators.end()) {
        operators.erase(it);
        std::cout << "Client <" << fd << "> removed from operators list." << std::endl;
    } else
        std::cout << "Client <" << fd << "> is not an operator." << std::endl;
}

void Channel::addOperator(int fd) {
    if (operators.find(fd) != operators.end() && operators[fd]) {
        std::cout << "Client <" << fd << "> is already an operator." << std::endl;
        return;
    }

    operators[fd] = true;
    std::cout << "Client <" << fd << "> has been added as an operator." << std::endl;
}

void Channel::removeOperator(int fd) {
    std::map<int, bool>::iterator it = operators.find(fd);
    if (it == operators.end() || !it->second) {
        std::cout << "Client <" << fd << "> is not an operator." << std::endl;
        return;
    }
    operators.erase(it);
    std::cout << "Client <" << fd << "> has been removed as an operator." << std::endl;
}

void Channel::broadcastMessage(const std::string& message, int senderFd) {
    for (size_t i = 0;i < clients.size(); i++)
        if (clients[i].getFd() != senderFd)
            if (send(clients[i].getFd(), message.c_str(), message.size(), 0) == -1) 
                std::cerr << "Send failed" << std::endl;
}

void Channel::handleKick(int operatorFd, int targetFd) {
    // Check if the operatorFd is an operator
    if (!isOperator(operatorFd)) {
        // Notify the operator that they do not have permission
        std::string errorMsg = "Error: You do not have the permission to kick users.\n";
        send(operatorFd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    // Check if the targetFd is a member of the channel
    if (!isClientInChannel(targetFd)) {
        // Notify the operator that the target client is not in the channel
        std::string errorMsg = "Error: The specified user is not in the channel.\n";
        send(operatorFd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    // Notify the targetFd that they have been kicked
    std::string kickMsg = "You have been kicked from the channel by an operator.\n";
    send(targetFd, kickMsg.c_str(), kickMsg.size(), 0);

    // Notify all remaining clients in the channel
    std::ostringstream oss;
    oss << "User " << targetFd 
    << " has been kicked from the channel by Operator " 
    << operatorFd << ".\n";
std::string broadcastMsg = oss.str();
    broadcastMessage(broadcastMsg, operatorFd);

    // Remove the client from the channel
    removeClient(targetFd);
}

void Channel::handleInvite(int operatorFd, int targetFd) {
    // Check if the operatorFd is an operator
    if (!isOperator(operatorFd)) {
        // Notify the operator that they do not have permission
        std::string errorMsg = "Error: You do not have the permission to invite users.\n";
        send(operatorFd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    // Check if the targetFd is already in the channel
    if (isClientInChannel(targetFd)) {
        // Notify the operator that the target user is already in the channel
        std::string errorMsg = "Error: The user is already a member of the channel.\n";
        send(operatorFd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    // Check if the channel is in invite-only mode
    if (mode['i'] && !isOperator(operatorFd)) {
        // Notify the operator that the channel is invite-only
        std::string errorMsg = "Error: The channel is invite-only. Only operators can invite users.\n";
        send(operatorFd, errorMsg.c_str(), errorMsg.size(), 0);
        return;
    }

    // Notify the targetFd of the invitation
    // std::string inviteMsg = "You have been invited to join the channel by Operator " +
    //                         std::to_string(operatorFd) + ".\n";
    std::ostringstream os;
    os << "You have been invited to join the channel by Operator " << operatorFd << ".\n";
    std::string inviteMsg = os.str();
    send(targetFd, inviteMsg.c_str(), inviteMsg.size(), 0);

    // Notify the operator that the invitation was sent
    // std::string confirmMsg = "User " + std::to_string(targetFd) +
    //                          " has been invited to the channel.\n";
    std::ostringstream oss;
    oss << "User " << targetFd << " has been invited to the channel.\n";
    std::string confirmMsg = oss.str();
    send(operatorFd, confirmMsg.c_str(), confirmMsg.size(), 0);
}

void Channel::handleMode(int operatorFd, const std::string& mode) {
    // Validate operator
    if (!isOperator(operatorFd)) {
        throw std::runtime_error("Error: Only operators can change channel modes.");
    }

    // Parsing the mode string
    bool addMode = true; // Default to add mode (+)
    size_t i = 0;

    while (i < mode.size()) {
        char flag = mode[i];

        if (flag == '+') {
            addMode = true;
        } else if (flag == '-') {
            addMode = false;
        } else if (mode.find(flag) != std::string::npos) {
            // Update the specific mode
            switch (flag) {
            case 'i': // Invite-only mode
            case 't': // Topic restriction
            case 'o': // Operator privilege
                this->mode[flag] = addMode;
                break;

            case 'k': // Key (password) restriction
                if (addMode) {
                    if (i + 1 >= mode.size())
                        throw std::runtime_error("Error: Key mode requires an argument.");
                    this->channelKey = mode.substr(i + 1);
                    i = mode.size(); // Skip the rest of the string
                } else {
                    this->channelKey.clear();
                }
                break;

            case 'l': // User limit
                if (addMode) {
                    if (i + 1 >= mode.size())
                        throw std::runtime_error("Error: Limit mode requires an argument.");
                    std::istringstream iss(mode.substr(i + 1));
                    int value;
                    if (iss >> value) {
                        this->userLimit = value;
                    } else {
                        throw std::invalid_argument("Invalid integer format");
                    }
                    i = mode.size(); // Skip the rest of the string
                } else {
                    this->userLimit = 0; // Remove user limit
                }
                break;

            default:
                throw std::runtime_error("Error: Invalid mode flag.");
            }
        }

        ++i;
    }

    // Notify clients about the mode change
    std::string modeChange = "Channel mode updated: " + mode;
    broadcastMessage(modeChange, operatorFd);
}
