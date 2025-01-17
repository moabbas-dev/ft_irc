/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 19:39:19 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/17 13:41:16 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Errors.hpp"
#include "../../includes/Cmd.hpp"

std::string getReason(std::string param) {
	std::istringstream istream(param);
	std::string reason,str;
	istream >> str;
	if (reason.empty())
		return std::string("Quit");
	if (reason[0] != ':') {
		for (size_t i = 0; i < reason.size(); i++){
			if (reason[i] == ' ') {
				reason.erase(reason.begin() + i, reason.end());
				break;
			}
		}
		reason.insert(reason.begin(), ':');
	}
	return reason;
}

void updateOtherClientChannels(Channel & channel, Client& client, Server& server) {
    std::vector<Client> &oldClients = channel.getClients();
    for (size_t i = 0;i < oldClients.size();i++) {
        if (oldClients[i].getFd() == client.getFd())
            continue;
        std::vector<Channel> channels = oldClients[i].getChannels();
		for(size_t i = 0;i < channels.size(); i++) {
			if (channels[i].getName() == channel.getName()) {
				channels[i].removeClient(client.getFd());
				channels[i].removeOperator(client.getFd());
			}
		}
        oldClients[i].setChannels(channels);
        std::map<int, Client>& server_clients = server.getClients();
        server_clients[oldClients[i].getFd()] = oldClients[i];
    }
}

void Cmd::QUIT(const Cmd& cmd, Server& server, Client& client) {
	std::string messageArgs[] = {client.getNickname(), ""};
	if (!client.getIsAuthenticated())
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTREGISTERED), (void)0);
	std::string reason = !cmd.getParams().empty()? getReason(cmd.getParams()[0]) : "";
    std::map<std::string, Channel>& channels = server.getChannels();
    int fd = client.getFd();

    for (std::map<std::string, Channel>::iterator channel = channels.begin(); channel != channels.end();) {
        Channel& currentChannel = channel->second;
        bool isClientInChannel = currentChannel.isClientInChannel(fd);
        bool isOperatorInChannel = currentChannel.isOperator(fd);

        if (isClientInChannel || isOperatorInChannel) {
            if (isClientInChannel)
                currentChannel.removeClient(fd);

            if (isOperatorInChannel)
                currentChannel.removeOperator(fd);

            if (currentChannel.getClients().empty()) {
				std::map<std::string, Channel>::iterator next = channel;
				next++;
                channels.erase(channel);
				channel = next;
                continue;
            } else {
                std::string rpl = ":" + client.getNickname() + "!~" + client.getUsername() + "@localhost QUIT :" + reason + "\r\n";
                currentChannel.broadcastMessage(rpl, fd);
            }
        }
		updateOtherClientChannels(currentChannel, client, server);
        ++channel;
    }
	std::ostringstream msg;
	msg << client.getNickname() << "<" << fd << "> disconnected.";
	Server::printResponse(msg.str(), RED);
	server.removeClientFromChannels(fd);
	server.clearClient(fd);
	close(fd);
}