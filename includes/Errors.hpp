/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 16:38:49 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/05 18:58:07 by moabbas          ###   ########.fr       */
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
		static bool commandFound(const std::string &command);
		static bool validParameters(Cmd &cmd, Client& client, Server &server);
};

std::vector<std::string> split(const std::string& str, char delimiter);

#endif