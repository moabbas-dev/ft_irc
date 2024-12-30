/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 16:38:49 by moabbas           #+#    #+#             */
/*   Updated: 2024/12/30 18:49:48 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <map>
#include <string>
#include <vector>
#include "../includes/Client.hpp"

enum ErrorCodes {
	// General
	ERR_NEEDMOREPARAMS,
	ERR_UNKNOWCOMMAND,
	ERR_TOOMANYPARAMS,

	// Authentication (PASS, USER)
	ERR_NOTREGISTERED ,
	ERR_ALREADYREGISTERED ,

	// PASS
	ERR_PASSWDMISMATCH ,
	
	// NICK
	ERR_NONICKNAMEGIVEN,
	ERR_ERRONEUSNICKNAME,
	ERR_NICKNAMEINUSE,

	// JOIN
	ERR_NOSUCHCHANNEL,
	ERR_TOOMANYCHANNELS,
	ERR_BADCHANNELKEY,
	ERR_CHANNELISFULL,
	ERR_INVITEONLYCHAN,
	
};

class Errors {
	private:
		static std::map<int, std::string> errors;

		static bool checkPASS(const std::string &command, const std::vector<std::string> &params, Client &client);
		static bool checkUSER(const std::string &command, const std::vector<std::string> &params, Client &client);
		static bool checkJOIN(const std::string &command, const std::vector<std::string> &params, Client &client);
		static bool checkNICK(const std::string &command, const std::vector<std::string> &params, Client &client);
		static bool checkPART(const std::string &command, const std::vector<std::string> &params, Client &client);
		static bool checkPING(const std::string &command, const std::vector<std::string> &params, Client &client);
		static bool checkPRIVMSG(const std::string &command, const std::vector<std::string> &params, Client &client);
		static bool checkKICK(const std::string &command, const std::vector<std::string> &params, Client &client);
		static bool checkINVITE(const std::string &command, const std::vector<std::string> &params, Client &client);
		static bool checkTOPIC(const std::string &command, const std::vector<std::string> &params, Client &client);
		static bool checkMODE(const std::string &command, const std::vector<std::string> &params, Client &client);
		

	public:
		static void raise(std::string clientName, std::string msgName, int errorCode);
		static bool commandFound(std::string command);
		static bool validParameters(const std::string &command, const std::vector<std::string> &params, Client& client);
};

#endif