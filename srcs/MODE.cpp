/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 19:06:07 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/06 14:29:53 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"
#include "../includes/Errors.hpp"
#include "../includes/StringUtils.hpp"
#include "../includes/Server.hpp"

// found insde Client.hpp but we need to check it before
static bool isOperatorInChannel(const std::string &channelName, Client &client, Server &server)
{
	std::map<std::string, Channel> &serverChannels = server.getChannels();
	for (std::map<std::string, Channel>::iterator it = serverChannels.begin(); it != serverChannels.end(); ++it)
	{
		if (it->first == channelName)
		{
			const std::map<int, bool> &operators = it->second.getOperators();
			std::map<int, bool>::const_iterator op_it = operators.begin();
			while (op_it != operators.end())
			{
				if (op_it->first == client.getFd())
				{
					if (op_it->second)
						return true;
				}
				++op_it;
			}
		}
	}
	return false;
}

// MODE util, We can let it here! But we can create ModeUtils.hpp and set it inside the file
static std::vector<char> getStringsSymbols(const std::string &modeStr)
{
	std::vector<char> symbols;
	for (size_t i = 0; i < modeStr.length(); ++i)
	{
		if (modeStr[i] == '+' || modeStr[i] == '-')
			symbols.push_back(modeStr[i]);
	}
	return symbols;
}

static bool checkDuplications(const std::vector<std::string> &modeSubStrs, const std::vector<char> &symbols)
{
	for (std::vector<std::string>::const_iterator it = modeSubStrs.begin(); it != modeSubStrs.end(); ++it)
	{
		if (hasDuplicates(*it))
			return false;
	}
	std::vector<std::string> setStrings;
	std::vector<std::string> removeStrings;
	for (size_t i = 0; i < symbols.size(); ++i)
	{
		if (symbols[i] == '+')
			setStrings.push_back(modeSubStrs[i]);
		else if (symbols[i] == '-')
			removeStrings.push_back(modeSubStrs[i]);
	}
	if (hasUniqueCharsInEachString(setStrings) && hasUniqueCharsInEachString(removeStrings))
		return true;
	return false;
}

static bool isCorrectModeString(const std::string &modeString)
{
	if (modeString.empty() || modeString.size() == 1)
		return false;
	if (modeString[0] != '-' && modeString[0] != '+')
		return false;
	for (size_t i = 1; i < modeString.size(); ++i)
	{
		if (modeString[i] != '+' && modeString[i] != '-' && modeString[i] != 'i'
			&& modeString[i] != 't' && modeString[i] != 'k' &&
				modeString[i] != 'o' && modeString[i] != 'l')
			return false;
		if ((modeString[i] == '+' || modeString[i] == '-') &&
            (i == modeString.size() - 1 || modeString[i + 1] == '+' || modeString[i + 1] == '-'))
            return false;
	}
	std::vector<std::string> modeSubStrs = split(modeString, "+-");
	std::vector<char> symbols = getStringsSymbols(modeString);
	if (!checkDuplications(modeSubStrs, symbols))
		return false;
	return true;
}

static size_t getNbOfModeArguments(const std::string &modeString)
{
	bool set = true;
	size_t i = 0;
	size_t nbArguments = 0;
	while (i < modeString.size())
	{
		if (modeString[i] == '+' || modeString[i] == '-')
		{
			if (modeString[i] == '+')
				set = true;
			else
				set = false;
		}
		else
		{
			if ((set && (modeString[i] == 'k' || modeString[i] == 'o' || modeString[i] == 'l')) ||
				(!set && modeString[i] == 'o'))
				nbArguments++;
		}
		++i;
	}
	return nbArguments;
}

bool Errors::checkMODE(Cmd &cmd, Client &client, Server &server)
{
	std::string messageArgs[] = {client.getNickname(), "", ""};
	if (!client.getIsAuthenticated())
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTREGISTERED), false);
	if (cmd.getParams().size() < 1)
		return (Server::sendError(messageArgs, client.getFd(), ERR_NOTENOUGHPARAM), false);
	if (cmd.getParams().size() == 1)
	{
		messageArgs[0] = client.getUsername();
		messageArgs[1] = cmd.getParams()[0];
		if (!server.channelExistInServer(cmd.getParams()[0]))
			return (Server::sendError(messageArgs, client.getFd(), ERR_NOSUCHCHANNEL), false);
		return true;
	}

	messageArgs[0] = cmd.getParams()[0];
	if (!isOperatorInChannel(cmd.getParams()[0], client, server))
		return (Server::sendError(messageArgs, client.getFd(), ERR_CHANOPRIVSNEEDED), false);

	messageArgs[0] = client.getNickname(); messageArgs[1] = cmd.getParams()[0]; messageArgs[2] = cmd.getParams()[1];
	if (!isCorrectModeString(cmd.getParams()[1]))
		return (Server::sendError(messageArgs, client.getFd(), ERR_UNKNOWNMODE), false);
	if (cmd.getParams().size() < (2 + getNbOfModeArguments(cmd.getParams()[1])))
		return(Server::sendError(messageArgs, client.getFd(), ERR_NOTENOUGHPARAM), false);
	return true;
}

static bool isValidLimitString(const std::string &str, Channel *channel)
{
	std::istringstream ss(str);
	int value;

	if (!channel)
		return false;
	if (str.empty())
		return false;
	if (!(ss >> value))
		return false;
	if (!ss.eof())
		return false;
	if (value <= 0 || value < static_cast<int>(channel->getClients().size()))
		return false;
	return true;
}

static void sendInvalidParameterMessage(Client &client, const Cmd &cmd, const std::string &modeStr)
{
	std::string msg = ": 472 " + client.getNickname() + " " + modeStr;
	for (size_t i = 2; i < cmd.getParams().size(); ++i)
	{
		msg += cmd.getParams()[i];
		msg += " ";
	}
	msg += ":Invalid parameter\n";
	send(client.getFd(), msg.c_str(), msg.size(), 0);
}

static void handleSingleArgument(const Cmd &cmd, Client &client, Channel *channel)
{
	if (!channel)
		return;
	std::map<char, bool> &mode = channel->getMode();
	std::string msg324 = ": 324 " + client.getNickname() + " " +
		cmd.getParams()[0] + " ";
	std::string msg329 = ": 329 " + client.getNickname() + " " +
		cmd.getParams()[0] + " " +
			getNumberAsString((time_t)channel->getCreationTime()) + "\n";
	for (std::map<char, bool>::iterator it = mode.begin(); it != mode.end(); ++it)
	{
		if (it->second)
		{
			msg324 += "+";
			break;
		}
	}
	for (std::map<char, bool>::iterator it = mode.begin(); it != mode.end(); ++it)
	{
		if (it->second)
			msg324 += it->first;
	}
	if (msg324.find('+') != std::string::npos)
		msg324 += " ";
	for (std::map<char, bool>::iterator it = mode.begin(); it != mode.end(); ++it)
	{
		if (it->second)
		{
			switch (it->first)
			{
				case 'k':
					msg324 += channel->getChannelKey();
					break;
				case 'l':
					msg324 += getNumberAsString((int)channel->getUserLimit());
					break;
				default:
					break;
			}
		}
		if (it != mode.end())
			msg324 += " ";
	}
	msg324 += "\n";
	send(client.getFd(), msg324.c_str(), msg324.size(), 0);
	send(client.getFd(), msg329.c_str(), msg329.size(), 0);
}

static void handleMultipleArguments(const Cmd &cmd, Client &client, Channel *channel, Server &server)
{
	if (!channel)
		return ;
	std::map<char, bool> &mode = channel->getMode();
	bool set = true;
	std::string modeStr = cmd.getParams()[1];
	std::string msgToSend = ":" + client.getNickname() + " MODE " + cmd.getParams()[0] + " ";
	std::string modeNewStr = ""; // MODE #channel modeStr modeArgs
	size_t argsIndex = 2;
	std::vector<std::string> validArgs;
	
	for (size_t i = 0; i < modeStr.size(); ++i)
	{
		if (modeStr[i] == '+' || modeStr[i] == '-')
		{
			set = modeStr[i] == '+' ? true : false;
			modeNewStr += modeStr[i];
		}
		else
		{
			switch(modeStr[i])
			{
				case 'i':
					mode['i'] = set;
					channel->setInviteOnly(set);
					modeNewStr += "i";
					break;
				case 't':
					mode['t'] = set;
					channel->setTopicRestricted(!set);
					modeNewStr += "t";
					break;
				case 'l':
					if (!isValidLimitString(cmd.getParams()[argsIndex], channel))
					{
						sendInvalidParameterMessage(client, cmd, modeStr);
						continue;
					}
					mode['l'] = set;
					if (set)
					{
						std::istringstream ss(cmd.getParams()[argsIndex]);
						int value;
						ss >> value;
						validArgs.push_back(cmd.getParams()[argsIndex]);
						++argsIndex;
						channel->setUserLimit(value);
						channel->setHasUserLimit(true);
					}
					else
					{
						channel->setUserLimit(-1);
						channel->setHasUserLimit(false);
					}
					modeNewStr += "l";
					break;
				case 'k':
					if (!isValidPass(cmd.getParams()[argsIndex]))
					{
						sendInvalidParameterMessage(client, cmd, modeStr);
						continue;
					}
					mode['k'] = set;
					if (set)
					{
						std::string key = cmd.getParams()[argsIndex];
						validArgs.push_back(key);
						++argsIndex;
						channel->setHasKey(true);
						channel->setChannelKey(key);
					}
					else
					{
						channel->setHasKey(false);
						channel->setChannelKey("");
					}
					modeNewStr += "k";
					break;
				case 'o':
					if (!server.clientIsInServer(cmd.getParams()[argsIndex]))
					{
						std::string msg = ": 401 " + client.getNickname() + " "
							+ channel->getName() + " :No such nick\n";
						std::cout << msg << std::endl;
						send(client.getFd(), msg.c_str(), msg.size(), 0);
						continue;
					}
					if (!channel->isClientInChannel(client.getFd()))
					{
						std::string msg = ": 441 " + client.getNickname() + " "
							+ channel->getName() + " :They aren't on that channel\n";
						std::cout << msg << std::endl;
						send(client.getFd(), msg.c_str(), msg.size(), 0);
						continue;
					}
					std::map<int, bool> &operators = channel->getOperators();
					const std::map<int, Client> &clients = server.getClients();
					if (set)
					{
						std::string nick = cmd.getParams()[argsIndex];
						validArgs.push_back(nick);
						++argsIndex;
						for (std::map<int, Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
						{
							if (it->second.getNickname() == nick)
							{
								operators[it->first] = true;
								break;
							}
						}
					}
					else
					{
						std::string nick = cmd.getParams()[argsIndex];
						validArgs.push_back(nick);
						++argsIndex;
						for (std::map<int, Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
						{
							if (it->second.getNickname() == nick)
							{
								operators[it->first] = false;
								break;
							}
						}
					}
					modeNewStr += "o";
					break;
			}
		}
	}
	std::string msg = ":" + client.getHostName() + " MODE " + channel->getName()
		+ " " + modeNewStr + " ";
	for (size_t i = 0; i < validArgs.size(); ++i)
	{
		msg += validArgs[i];
		if (i != validArgs.size() - 1)
			msg += " ";
	}
	msg += "\r\n";
	std::cout << msg << std::endl;
	std::vector<Client> &clients = channel->getClients();
	for (size_t i = 0; i < clients.size(); ++i)
	{
		send(clients[i].getFd(), msg.c_str(), msg.size(), 0);
	}
}

void Cmd::MODE(const Cmd& cmd, Server& server, Client& client)
{
	size_t args = cmd.getParams().size();
	std::string channelName = cmd.getParams()[0];
	Channel *channel = server.getSpecifiedChannel(channelName);
	if (!channel)
	{
		// aya shi
		std::string messageArgs[] = {client.getNickname(), cmd.getName()};
		Server::sendError(messageArgs, client.getFd(), ERR_NOSUCHCHANNEL);
		return ;
	}
	if (args == 1)
		handleSingleArgument(cmd, client, channel);
	else
		handleMultipleArguments(cmd, client, channel, server);
}
