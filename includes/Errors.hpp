/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 16:38:49 by moabbas           #+#    #+#             */
/*   Updated: 2024/12/30 18:01:45 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <map>
#include <string>
#include <vector>

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

	public:
		static void raise(std::string clientName, std::string msgName, int errorCode);
		static bool commandFound(std::string command);
		static bool validParameters(std::string command, const std::vector<std::string> params, Client& client);
};

#endif