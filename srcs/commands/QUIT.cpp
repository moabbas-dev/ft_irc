/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 19:39:19 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/06 13:15:41 by moabbas          ###   ########.fr       */
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

void Cmd::QUIT(const Cmd& cmd, Server& server, Client& client) {
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
        ++channel;
    }

	std::ostringstream msg;
	msg << client.getNickname() << "<" << fd << "> disconnected.";
	Server::printResponse(msg.str(), RED);
	server.removeClientFromChannels(fd);
	server.clearClient(fd);
	close(fd);
}