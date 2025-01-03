/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PART.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:48:10 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/03 16:55:54 by afarachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"
#include "../includes/Errors.hpp"

void Cmd::PART(const Cmd& cmd, Server& server, Client& client) {

    std::vector<std::string> splitted_params = split(cmd.getParams()[0], ',');
    std::vector<std::string>::iterator it = splitted_params.begin();

    while (it != splitted_params.end()) {
        std::string channel_name = *it;

        Channel* channel = server.findChannel(channel_name);

        channel->removeClient(client.getFd());

        std::string reason = (cmd.getParams().size() > 1) ? cmd.getParams()[1] : "";
        std::string part_message = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostName()
            + " PART " + channel_name;
        if (!reason.empty())
            part_message += " :" + reason;

        channel->broadcastMessage(part_message, client.getFd());

        if (channel->isEmpty())
            server.deleteChannel(channel_name);

        ++it;
    }
}



