/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 19:06:07 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/05 15:41:03 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"
#include "../includes/Errors.hpp"

// inside Server.hpp (to be implemented inside the class Server)
static bool channelExistInServer(const std::string &channelName, Server &server)
{
	std::map<std::string, Channel> &serverChannels = server.getChannels();
	for (std::map<std::string, Channel>::iterator it = serverChannels.begin(); it != serverChannels.end(); ++it)
	{
		if (it->first == channelName)
			return true;
	}
	return false;
}

// inside Client.hpp (to be implemented inside the class Client)
static bool isInsideTheChannel(Channel &channel, Client &client)
{
	std::vector<Channel> &channels = client.getChannels();
	for (size_t i = 0; i < channels.size(); ++i)
	{
		if (channel.getName() == channels[i].getName())
			return true;
	}
	return false;
}

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

// to be implemented later inside utils.hpp (or stringUtils.hpp)
static std::vector<std::string> split(const std::string& str, const std::string& delimiters)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find_first_of(delimiters);

    while (end != std::string::npos)
	{
        if (end != start)
            tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find_first_of(delimiters, start);
    }
	
    if (start < str.length()) {
        tokens.push_back(str.substr(start));
    }

    return tokens;
}

// to be implemented inside stringUtils.hpp
static bool hasDuplicates(const std::string& str)
{
    for (size_t i = 0; i < str.length(); ++i)
	{
        for (size_t j = i + 1; j < str.length(); ++j)
		{
            if (str[i] == str[j])
                return true;
        }
    }
    return false;
}

// MODE #channel modeStr modeArguments
// modeStr = -kl+i
// vector : kl, i
// symbolsVector : -, +
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

// MODE util, We can let it here! But we can create ModeUtils.hpp and set it inside the file
static bool hasUniqueCharsInEachString(const std::vector<std::string> &vec)
{
    for (size_t i = 0; i < vec.size(); ++i)
	{
        const std::string& str1 = vec[i];
        for (size_t j = 0; j < vec.size(); ++j)
		{
            if (i != j)
			{
                const std::string& str2 = vec[j];
                for (size_t k = 0; k < str1.length(); ++k)
				{
                    for (size_t l = 0; l < str2.length(); ++l)
					{
                        if (str1[k] == str2[l])
                            return false;
                    }
                }
            }
        }
    }
    return true;
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

// MODE #channel modeStr modeArgs (0, 1, 2, 3, ...)
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
	if (!client.getIsAuthenticated())
		return (raise(client, "", ERR_NOTREGISTERED), false);
	if (cmd.getParams().size() < 1)
		return (raise(client, "", ERR_NEEDMOREPARAMS), false);
	if (cmd.getParams().size() == 1)
	{
		if (!channelExistInServer(cmd.getParams()[0], server))
			return (raise(client, "", ERR_NOSUCHCHANNEL), false);
		return true;
	}
	if (!isOperatorInChannel(cmd.getParams()[0], client, server))
		return (raise(client, cmd.getParams()[0], ERR_CHANOPRIVSNEEDED), false);
	if (!isCorrectModeString(cmd.getParams()[1]))
		return (raise(client, cmd.getParams()[1], ERR_UNKNOWNMODE), false);
	if (cmd.getParams().size() < (2 + getNbOfModeArguments(cmd.getParams()[1])))
		return(raise(client, "", ERR_NEEDMOREPARAMS), false);
	return true;
}

static Channel &getSpecifiedChannel(const Cmd &cmd, Server &server)
{
	std::map<std::string, Channel> &channels = server.getChannels();
	if (channels.empty())
		throw std::exception();
	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->first == cmd.getParams()[0])
			return it->second;
	}
	throw std::exception();
}

// stringUtils.hpp 
static std::string getNumberAsString(unsigned long long nb)
{
	std::ostringstream oss;
    oss << nb;
    return oss.str();
}

// ModeUtils.hpp
static bool isValidLimitString(const std::string &str, Channel &channel)
{
	std::istringstream ss(str);
	int value;

	if (str.empty())
		return false;
	if (!(ss >> value))
		return false;
	if (!ss.eof())
		return false;
	if (value <= 0 || value < static_cast<int>(channel.getClients().size()))
		return false;
	return true;
}

// stringUtils.hpp
static bool isValidPass(const std::string &pass)
{
	if(pass.empty())
		return false;
	for (size_t i = 0; i < pass.size(); ++i)
	{
		if (!std::isalnum(pass[i]))
			return false;
	}
	return true;
}

// 
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

static bool clientIsInServer(const std::string &clientNick, Server &server)
{
	const std::map<int, Client> &clients = server.getClients();
	for (std::map<int, Client>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second.getNickname() == clientNick)
			return true;
	}
	return false;
}

static void handleSingleArgument(const Cmd &cmd, Client &client, Channel &channel)
{
	std::map<char, bool> &mode = channel.getMode();
	std::string msg324 = ": 324 " + client.getNickname() + " " +
		cmd.getParams()[0] + " ";
	std::string msg329 = ": 329 " + client.getNickname() + " " +
		cmd.getParams()[0] + " " +
			getNumberAsString((time_t)channel.getCreationTime()) + "\n";
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
					msg324 += channel.getChannelKey();
					break;
				case 'l':
					msg324 += getNumberAsString((int)channel.getUserLimit());
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

static void handleMultipleArguments(const Cmd &cmd, Client &client, Channel &channel, Server &server)
{
	std::map<char, bool> &mode = channel.getMode();
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
					channel.setInviteOnly(set);
					modeNewStr += "i";
					break;
				case 't':
					mode['t'] = set;
					channel.setTopicRestricted(!set);
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
						channel.setUserLimit(value);
						channel.setHasUserLimit(true);
					}
					else
					{
						channel.setUserLimit(-1);
						channel.setHasUserLimit(false);
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
						channel.setHasKey(true);
						channel.setChannelKey(key);
					}
					else
					{
						channel.setHasKey(false);
						channel.setChannelKey("");
					}
					modeNewStr += "k";
					break;
				case 'o':
					if (!clientIsInServer(cmd.getParams()[argsIndex], server))
					{
						std::string msg = ": 401 " + client.getNickname() + " " +
							cmd.getParams()[argsIndex] + " :No such nick\n";
						send(client.getFd(), msg.c_str(), msg.size(), 0);
						continue;
					}
					if (!isInsideTheChannel(channel, client))
					{
						std::string msg = ": 441 " + client.getNickname() + " "
							+ channel.getName() + " :They aren't on that channel\n";
						send(client.getFd(), msg.c_str(), msg.size(), 0);
						continue;
					}
					std::map<int, bool> &operators = channel.getOperators();
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
	std::string msg = ":" + client.getHostName() + " MODE " + channel.getName()
		+ " " + modeNewStr + " ";
	for (size_t i = 0; i < validArgs.size(); ++i)
	{
		msg += validArgs[i];
		if (i != validArgs.size() - 1)
			msg += " ";
	}
	msg += "\r\n";
	std::cout << msg << std::endl;
	std::vector<Client> &clients = channel.getClients();
	for (size_t i = 0; i < clients.size(); ++i)
	{
		send(clients[i].getFd(), msg.c_str(), msg.size(), 0);
	}
}

void Cmd::MODE(const Cmd& cmd, Server& server, Client& client) {
	try
	{
		size_t args = cmd.getParams().size();
		Channel &channel = getSpecifiedChannel(cmd, server);
		if (args == 1)
			handleSingleArgument(cmd, client, channel);
		else
			handleMultipleArguments(cmd, client, channel, server);
	}
	catch(std::exception &)
	{
		Errors::raise(client, "", ERR_NOSUCHCHANNEL);
	}
}
