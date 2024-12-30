/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 16:45:21 by moabbas           #+#    #+#             */
/*   Updated: 2024/12/30 18:01:37 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Errors.hpp"
#include "../includes/Cmd.hpp"

std::map<int, std::string> Errors::errors;

void Errors::raise(std::string clientName, std::string msgName, int errorCode) {
	std::string result = msgName.empty()? clientName + " " : clientName + " " + msgName+ " ";
	switch(errorCode) {
		case ERR_UNKNOWCOMMAND:
			result.append(":Unknow Command");
		case ERR_NEEDMOREPARAMS:
			result.append(":Not enough parameters");
		case ERR_TOOMANYPARAMS:
			result.append(":Too many parameters");
		case ERR_PASSWDMISMATCH:
			result.append(":Password incorrect");
		case ERR_ALREADYREGISTERED:
			result.append(":You may not reregister");
		case ERR_NOTREGISTERED:
			result.append(":You have not registered");
		case ERR_NONICKNAMEGIVEN:
			result.append(":No nickname given");
		case ERR_ERRONEUSNICKNAME:
			result.append(":Erroneus nickname");
		case ERR_NICKNAMEINUSE:
			result.append("Nickname is already in use");
	}
}

bool Errors::commandFound(std::string command) {
    return command == "PASS" || command == "JOIN"
        || command == "NICK" || command == "PART"
        || command == "PING" || command == "PRIVMSG"
        || command == "USER";
}

bool Errors::validParameters(std::string command, const std::vector<std::string> params, Client& client) {
    if (command == "PASS") {
        return checkPASS(command, params, client);
    } else if (command == "JOIN") {
        return checkJOIN(command, params, client);
    } else if (command == "NICK") {
        return checkNICK(command, params, client);
    } else if (command == "PART") {
        return checkPART(command, params, client);
    } else if (command == "PING") {
        return checkPING(command, params, client);
    } else if (command == "PRIVMSG") {
        return checkPRIVMSG(command, params, client);
    } else if (command == "USER") {
        return checkUSER(command, params, client);
    } else if (command == "KICK") {
		return checkKIK(command, params, client);
	} else if (command == "INVITE") {
		return checkINVITE(command, params, client);
	} else if (command == "TOPIC") {
		return checkTOPIC(command, params, client);
	} else if (command == "MODE") {
		return checkMODE(command, params, client);
	}
    return true;
}
