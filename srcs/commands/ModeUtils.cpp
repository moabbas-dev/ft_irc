/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeUtils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:39:33 by jfatfat           #+#    #+#             */
/*   Updated: 2025/01/20 21:47:00 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ModeUtils.hpp"
#include "../../includes/Channel.hpp"
#include "../../includes/Client.hpp"

std::vector<char> getStringsSymbols(const std::string &modeStr)
{
	std::vector<char> symbols;
	for (size_t i = 0; i < modeStr.length(); ++i)
	{
		if (modeStr[i] == '+' || modeStr[i] == '-')
			symbols.push_back(modeStr[i]);
	}
	return symbols;
}

bool checkDuplications(const std::vector<std::string> &modeSubStrs, const std::vector<char> &symbols)
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

bool isCorrectModeString(const std::string &modeString)
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

size_t getNbOfModeArguments(const std::string &modeString)
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

bool isValidLimitString(const std::string &str, Channel *channel)
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

std::string formatModeString(const std::string &modeStr)
{
	if (modeStr.empty())
		return std::string("");
	if (modeStr.size() == 1 && (modeStr[0] == '+' || modeStr[0] == '-'))
		return std::string("");
	std::string newModeStr = "";
	bool set = modeStr[0] == '+' ? true : false;
	newModeStr += modeStr[0];
	for (size_t i = 1; i < modeStr.size(); ++i)
	{
		if (i == modeStr.size() - 1)
		{
			if (modeStr[i] == '-' || modeStr[i] == '+')
				continue;
		}
		if (modeStr[i] == '+' || modeStr[i] == '-')
		{
			if (modeStr[i] == '+')
			{
				if (set)
					continue;
				if (modeStr[i + 1] == '-')
					continue;
				newModeStr += "+";
				set = true;
			}
			else
			{
				if (!set)
					continue;
				if (modeStr[i + 1] == '+')
					continue;
				newModeStr += "-";
				set = false;	
			}
		}
		else
			newModeStr += modeStr[i];
	}
	return newModeStr;
}

void separateAndBroadcast(Client &client, Channel *channel,
	const std::string &modeStr, std::vector<std::string> &args)
{
	bool set;
	size_t len = 0;
	std::vector<std::string> msgs;
	std::string msg;
	char symbol;

	if (!channel)
		return ;
	for (size_t i = 0; i < modeStr.size(); ++i)
	{
		if (modeStr[i] == '-' || modeStr[i] == '+')
			set = (modeStr[i] == '+');
		else if (modeStr[i] == 'i' || modeStr[i] == 't')
		{
			symbol = set ? '+' : '-';
			msg = ":" + client.getHostName() + " MODE " + channel->getName()
				+ " " + symbol + modeStr[i] + "\r\n";
			msgs.push_back(msg);
		}
		else if (modeStr[i] == 'k' || modeStr[i] == 'l')
		{
			symbol = set ? '+' : '-';
			msg = ":" + client.getHostName() + " MODE " + channel->getName()
				+ " " + symbol + modeStr[i];
			if (set)
			{
				msg = msg + " " + args[len];
				++len;	
			}
			msg += "\r\n";
			msgs.push_back(msg);
		}
		else if (modeStr[i] == 'o')
		{
			symbol = set ? '+' : '-';
			msg = ":" + client.getHostName() + " MODE " + channel->getName()
				+ " " + symbol + modeStr[i] + " " + args[len] + "\r\n";
			++len;
			msgs.push_back(msg);
		}
	}
	std::vector<Client> &clients = channel->getClients();
	for (size_t i = 0; i < clients.size(); ++i)
	{
		for (size_t j = 0; j < msgs.size(); ++j)
			send(clients[i].getFd(), msgs[j].c_str(), msgs[j].size(), 0);
	}
}