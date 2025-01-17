/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:38:28 by jfatfat           #+#    #+#             */
/*   Updated: 2025/01/17 14:38:29 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Bot.hpp"
#include "../includes/Client.hpp"

Bot::Bot() : Client()
{
	this->nickname = "marvin";
	this->username = "bot";
	this->realname = "Marvin the bot";
	this->setHasSetNickName(true);
}

Bot::~Bot() { }

void Bot::connectToServer(Server &server)
{
    struct sockaddr_in serverAddr;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

	std::string serverAddress = "127.0.0.1";
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server.getPort());
    serverAddr.sin_addr.s_addr = inet_addr(serverAddress.c_str());
	if (serverAddr.sin_addr.s_addr == INADDR_NONE) {
    	std::cerr << "Invalid address or address not supported" << std::endl;
    	return;
	}

    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return;
    }

    this->setFd(sockfd);
    std::cout << "Bot connected to server " << serverAddress << " on port " << server.getPort() << " with fd : " << sockfd << std::endl;
}

static bool isAValidDate(const std::string &date)
{
	std::tm tm;

	tm.tm_year = 0;
    tm.tm_mon = 0;
    tm.tm_mday = 0;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    tm.tm_isdst = -1;

	char sep1, sep2;
	std::istringstream ss(date);

	ss >> tm.tm_year >> sep1 >> tm.tm_mon >> sep2 >> tm.tm_mday;

	tm.tm_year -= 1900;
	tm.tm_mon -= 1;

	if (ss.fail() || sep1 != '/' || sep2 != '/' || tm.tm_year < 0
		|| tm.tm_mon < 0 || tm.tm_mon > 11 || tm.tm_mday < 1 || tm.tm_mday > 31)
		return false;
	
	if (!ss.eof())
		return false;
	
	const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	
	int year = tm.tm_year + 1900;
	int month = tm.tm_mon;
	
	if (month == 1)
	{
		int maxDay = (year % 4 == 0) ? 29 : 28;
		if (tm.tm_mday > maxDay)
			return false;
	}
	else
	{
		if (tm.tm_mday > daysInMonth[month])
			return false;
	}

    std::time_t t = std::time(0);
    std::tm current_tm = *std::localtime(&t);

    if (tm.tm_year > current_tm.tm_year)
        return false;
    if (tm.tm_year == current_tm.tm_year)
    {
        if (tm.tm_mon > current_tm.tm_mon)
            return false;
        if (tm.tm_mon == current_tm.tm_mon)
        {
            if (tm.tm_mday > current_tm.tm_mday)
                return false;
        }
    }
	return true;
}

static void calculateAge(const std::string &date, int *years, int *months, int *days)
{
    std::tm tm;
    tm.tm_year = 0;
    tm.tm_mon = 0;
    tm.tm_mday = 0;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    tm.tm_isdst = -1;

    char sep1, sep2;
    std::istringstream ss(date);
    ss >> tm.tm_year >> sep1 >> tm.tm_mon >> sep2 >> tm.tm_mday;

    tm.tm_year -= 1900;
    tm.tm_mon -= 1;

    std::time_t t = std::time(0);
    std::tm current_tm = *std::localtime(&t);

    *years = current_tm.tm_year - tm.tm_year;
    *months = current_tm.tm_mon - tm.tm_mon;
    *days = current_tm.tm_mday - tm.tm_mday;

    if (*days < 0)
    {
        --(*months);
        *days += (current_tm.tm_mon == 0 ? 31 : 30);
    }

    if (*months < 0)
    {
        --(*years);
        *months += 12;
    }
}

void Bot::sendAgeMsg(Client& client, const std::string &command)
{
	std::string ageMsg;
	std::vector<std::string> commandSplit = split(command, " ");
	int years, months, days;

	if (command == "!age" || (commandSplit.size() == 2 && !isAValidDate(commandSplit[1])))
		ageMsg = "I can't calculate your age! Please provide me with a valid age. Example : 2025/01/01\n";
	else
	{
		calculateAge(commandSplit[1], &years, &months, &days);
		std::ostringstream oss;
		oss << "Age: " << years << " year" << (years != 1 ? "s" : "") << ", "
			<< months << " month" << (months != 1 ? "s" : "") << ", " << days
			<< " day" << (days != 1 ? "s" : "") << "\n";
		ageMsg = oss.str();
	}
	std::string msg = ":" + this->getNickname() + "!~"
		+ this->getUsername() + "@localhost PRIVMSG "
		+ client.getNickname() + " :" + ageMsg + "\r\n";
	send(client.getFd(), msg.c_str(), msg.size(), 0);
}

void Bot::sendJokeMsg(Client& client)
{
	const char *jokes[] = 
	{
    	"Why don't scientists trust atoms? Because they make up everything!",
    	"Why did the scarecrow win an award? Because he was outstanding in his field!",
    	"Why don't skeletons fight each other? They don't have the guts!",
    	"What do you call cheese that isn't yours? Nacho cheese!",
    	"Why did the bicycle fall over? Because it was two-tired!",
    	"What did one wall say to the other wall? I'll meet you at the corner!",
    	"Why did the math book look sad? It had too many problems.",
    	"What do you call fake spaghetti? An impasta!",
    	"Why did the golfer bring two pairs of pants? In case he got a hole in one!",
    	"Why did the cookie go to the doctor? Because it felt crumby!"
	};

	srand(static_cast<unsigned int>(time(0)));
	int randomIndex = rand() % (sizeof(jokes) / sizeof(jokes[0]));
	
	std::string msg = ":" + this->getNickname() + "!~"
		+ this->getUsername() + "@localhost PRIVMSG "
		+ client.getNickname() + " :" + jokes[randomIndex] + "\r\n";
	send(client.getFd(), msg.c_str(), msg.size(), 0);
}

void Bot::sendHelpMsg(Client& client)
{
	const char *msgs[] =
	{
        "Here is the list of commands you can use on this server:\n",
        "1. KICK - Remove a client from a channel. Example: /kick <channel> <nickname> [<reason>]\n",
        "2. INVITE - Invite a client to join a channel. Example: /invite <nickname> <channel>\n",
        "3. TOPIC - View or set the topic of a channel. Example: /topic <channel> [<new_topic>]\n",
        "4. MODE - Change a channel's mode. Example: /mode <channel> (+ or -)<mode>\n",
        "   Available Modes:\n",
        "     - i: Set/remove invite-only mode.\n",
        "     - t: Restrict topic changes to channel operators.\n",
        "     - k: Set/remove a channel password.\n",
        "     - o: Grant/revoke operator privileges.\n",
        "     - l: Set/remove user limit for the channel.\n",
        "5. USER - Set your username and real name. Example: /user <username> <hostname> <servername> <realname>\n",
        "6. NICK - Set or change your nickname. Example: /nick <new_nickname>\n",
        "7. PART - Leave a channel. Example: /part <channel> [<reason>]\n",
        "8. JOIN - Join a channel. Example: /join <channel> [<pass_key>]\n",
        "9. PRIVMSG - Send a private or channel message. Example: /msg <target> <message>\n",
		"10. PING - Send a ping to check server connectivity. Example: /ping <server_name>\n"
    };
	size_t lines = sizeof(msgs) / sizeof(msgs[0]);
	for (size_t i = 0; i < lines; ++i)
	{
		std::string msg = ":" + this->getNickname() + "!~"
		+ this->getUsername() + "@localhost PRIVMSG "
		+ client.getNickname() + " :" + msgs[i] + "\r\n";
		send(client.getFd(), msg.c_str(), msg.size(), 0);
	}
}

void Bot::sendUnknowCmdMsg(Client& client)
{
	const char *msgs[] = 
	{
		"Sorry! I can't understand what did you mean, I can answer only these three messages:\n",
		"1. !help - Give you a list of the commands that you can use in this server, each one provided with an example\n",
		"2. !joke - Give you a random joke from the server\n",
		"3. !age date(xxxx-xx-xx) - Give you the age depending on the date\n"
	};
	size_t lines = sizeof(msgs) / sizeof(msgs[0]);
	for (size_t i = 0; i < lines; ++i)
	{
		std::string msg = ":" + this->getNickname() + "!~"
		+ this->getUsername() + "@localhost PRIVMSG "
		+ client.getNickname() + " :" + msgs[i] + "\r\n";
		send(client.getFd(), msg.c_str(), msg.size(), 0);
	}
}
