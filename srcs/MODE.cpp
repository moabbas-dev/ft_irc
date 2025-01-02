/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/31 19:06:07 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/01 21:53:58 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"
#include "../includes/Errors.hpp"

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
	if (modeString.empty())
		return false;
	if (modeString[0] != '-' && modeString[0] != '+')
		return false;
	for (size_t i = 1; i < modeString.size(); ++i)
	{
		if (modeString[i] != '+' && modeString[i] != '-' && modeString[i] != 'i'
			&& modeString[i] != 't' && modeString[i] != 'k' &&
				modeString[i] != 'o' && modeString[i] != 'l')
			return false;
	}
	for (size_t i = 0; i < modeString.size() - 1; ++i)
	{
		if (modeString[i] == modeString[i + 1])
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

static std::string getNumberAsString(unsigned long long nb)
{
	std::ostringstream oss;
    oss << nb;
    return oss.str();
}

void Cmd::MODE(const Cmd& cmd, Server& server, Client& client) {
	// (void)cmd;
	// (void)server;
	// (void)client;
	// MODE channel modeStr modeArgs
	// bool set = true;
	// std::string modeStr = cmd.getParams()[1];
	try
	{
		size_t args = cmd.getParams().size();
		Channel &channel = getSpecifiedChannel(cmd, server);
		std::map<char, bool> &mode = channel.getMode();
		if (args == 1)
		{
			std::string msg324 = ":localhost 324 " + client.getNickname() + " " +
				cmd.getParams()[0] + " ";
			std::string msg329 = ":localhost 329 " + client.getNickname() + " " +
				cmd.getParams()[0] + " " +
					getNumberAsString((time_t)channel.getCreationTime()) + "\n";
			for (std::map<char, bool>::iterator it = mode.begin(); it != mode.end(); ++it)
			{
				if (it->second && it->first != 'o')
				{
					msg324 += "+";
					break;
				}
			}
			for (std::map<char, bool>::iterator it = mode.begin(); it != mode.end(); ++it)
			{
				if (it->second && it->first != 'o')
					msg324 += it->first;
			}
			if (msg324.find('+') != std::string::npos)
				msg324 += " ";
			for (std::map<char, bool>::iterator it = mode.begin(); it != mode.end(); ++it)
			{
				if (it->second && it->first != 'o')
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
		else
		{
			return;
		}
	}
	catch(std::exception &)
	{
		Errors::raise(client, "", ERR_NOSUCHCHANNEL);
	}
	// TODO: Implement MODE command
}
