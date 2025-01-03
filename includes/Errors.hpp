/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 16:38:49 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/03 20:02:04 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <map>
#include <string>
#include <vector>
#include "../includes/Client.hpp"
#include "../includes/Server.hpp"
#include "../includes/Cmd.hpp"

enum ErrorCodes {
	// General
	ERR_NEEDMOREPARAMS = 461,
	ERR_UNKNOWCOMMAND = 421,
	ERR_TOOMANYPARAMS,

	// Authentication (PASS, USER)
	ERR_NOTREGISTERED = 451,
	ERR_ALREADYREGISTERED = 462,

	// PASS
	ERR_PASSWDMISMATCH = 464,
	
	// NICK
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICKNAME = 432,
	ERR_NICKNAMEINUSE = 433,

	// PART 
	ERR_NOTONCHANNEL = 442,

	// JOIN
	ERR_NOSUCHCHANNEL = 403,
	ERR_TOOMANYCHANNELS = 405,
	ERR_BADCHANNELKEY = 475,
	ERR_CHANNELISFULL,
	ERR_INVITEONLYCHAN,
	ERR_BADCHANMASK = 476,
	ERR_USERONCHANNEL = 443,
	RPL_TOPIC = 332,

	// MODE
	ERR_CHANOPRIVSNEEDED = 482,
	ERR_UNKNOWNMODE = 472
};

class Errors {
	private:
		static std::map<int, std::string> errors;
		static bool checkPASS(Cmd &cmd, Client &client, Server &server);
		static bool checkUSER(Cmd &cmd, Client &client);
		static bool checkJOIN(Cmd &cmd, Client &client);
		static bool checkNICK(Cmd &cmd, Client &client, Server &server);
		static bool checkPART(Cmd &cmd, Client &client, Server &server);
		static bool checkPING(Cmd &cmd, Client &client);
		static bool checkPRIVMSG(Cmd &cmd, Client &client);
		static bool checkKICK(Cmd &cmd, Client &client);
		static bool checkINVITE(Cmd &cmd, Client &client);
		static bool checkTOPIC(Cmd &cmd, Client &client);
		static bool checkMODE(Cmd &cmd, Client &client, Server &server);
		// static bool checkQUIT(Cmd &cmd, Client &client, Server &server);

	public:
		static void raise(Client& client, const std::string &msgName, int errorCode);
		static bool commandFound(const std::string &command);
		static bool validParameters(Cmd &cmd, Client& client, Server &server);
};

std::vector<std::string> split(const std::string& str, char delimiter);

#endif