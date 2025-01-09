/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/04 14:33:45 by moabbas           #+#    #+#             */
/*   Updated: 2025/01/09 21:17:39 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define once

#define BREAK "\r\n"

#define RPL_CONNECTED(nickname) (": 001 " + nickname + " :Welcome to the IRC server!" + BREAK)
#define RPL_CHANNELMODEIS(nickname, channelname, modes) ": 324 " + nickname + " " + channelname + " " + modes + BREAK
#define RPL_CREATIONTIME(nickname, channelname, creationtime) ": 329 " + nickname + " " + channelname + " " + creationtime + BREAK
#define RPL_NOTOPIC(nickname, channelname) (":localhost 331 " + nickname + " " + channelname + " :No topic is set" + BREAK)
#define RPL_TOPICIS(nickname, channelname, topic) (": 332 " + nickname + " " + channelname + " :" + topic + BREAK)
#define RPL_TOPICWHOTIME(clientname, channelname, nickname, setat) (": 333 " + clientname + " " + channelname + " " + nickname + " " + setat + BREAK)
#define RPL_NAMREPLY(nickname, channelname, clientslist) (": 353 " + nickname + " @ " + channelname + " :" + clientslist + BREAK)
#define RPL_ENDOFNAMES(nickname, channelname) (": 366 " + nickname + " " + channelname + " :END of /NAMES list" + BREAK)
#define RPL_UMODEIS(hostname, channelname, mode, user)  ":" + hostname + " MODE " + channelname + " " + mode + " " + user + BREAK
#define RPL_CHANGEMODE(hostname, channelname, mode, arguments) (":" + hostname + " MODE " + channelname + " " + mode + " " + arguments + BREAK)
#define RPL_NICKCHANGE(oldnickname, nickname) (":" + oldnickname + " NICK " + nickname + BREAK)
#define RPL_JOINMSG(hostname, ipaddress, channelname) (":" + hostname + "@" + ipaddress + " JOIN " + channelname + BREAK)
#define RPL_TOPICADMIN(nickname, username, channelname, topic) (":" + nickname + "!" + username + "@localhost TOPIC " + channelname + " :" + topic + BREAK)
#define RPL_NICKNAMECHANGED(nickname, username, newnickname) (":" + nickname + "!" + username + "@localhost NICK " + newnickname + BREAK)
#define RPL_NAMREPLY(nickname, channelname, clientslist) (": 353 " + nickname + " @ " + channelname + " :" + clientslist + BREAK)
#define RPL_ENDOFNAMES(nickname, channelname) (": 366 " + nickname + " " + channelname + " :END of /NAMES list" + BREAK)
#define RPL_INVITING(nickname, nickname2, channelname) (": 341 " + nickname + " " + nickname2 + " " + channelname + BREAK)
#define RPL_KICK(nickname, username, channelname, kicked_nickname, comment) (":" + nickname + "!~" + username + "@localhost" + " KICK " + channelname + " " +kicked_nickname + " :" + comment + BREAK)

#define ERR_NOSUCHNICK(channelname, name) (":localhost 401 " + channelname + " " + name + " :No such nick/channel" + BREAK )
#define ERR_NOSUCHCHANNEL(nickname, channelname) (": 403 " + nickname + " " + channelname + " :No such channel" + BREAK)
#define ERR_TOOMANYCHANNELS(nickname, channelname) (": 405 " + nickname + " " + channelname + " :You have joined too many channels" + BREAK)
#define ERR_CMDNOTFOUND(nickname, command) (": 421 " + nickname + " " + command + " :Unknown command" + BREAK)
#define ERR_NONICKNAMEGIVEN(nickname) (": 431 " + nickname + " :No nickname given" + BREAK )
#define ERR_ERRONEUSNICK(nickname) ": 432 " + nickname + " :Erroneus nickname" + BREAK
#define ERR_NICKINUSE(nickname) (": 433 " + nickname + " :Nickname is already in use" + BREAK)
#define ERR_NOTONTHATCHANNEL(nickname, channelname) (":localhost 441 " + nickname + " " + channelname + " :They aren't on that channel" + BREAK)
#define ERR_NOTONCHANNEL(nickname, channelname) (": 442 " + nickname + " " + channelname + " :You're not on that channel" + BREAK)
#define ERR_USERONCHANNEL(nickname, channelname) (": 443 " + nickname + " " + channelname + " :is already on channel" + BREAK)
#define ERR_NOTREGISTERED(nickname) (": 451 " + nickname + " :You have not registered!" + BREAK)
#define ERR_NOTENOUGHPARAM(nickname) (": 461 " + nickname + " :Not enough parameters." + BREAK)
#define ERR_ALREADYREGISTERED(nickname) (": 462 " + nickname + " :You may not reregister !" + BREAK )
#define ERR_PASSWDMISMATCH(nickname) (": 464 " + nickname + " :Password incorrect !" + BREAK )
#define ERR_KEYSET(channelname) ": 467 " + channelname + " Channel key already set. " + BREAK
#define ERR_CHANNELISFULL(nickname, channelname) (": 471 " + nickname + " " + channelname + " :Cannot join channel (+l)" + BREAK)
#define ERR_UNKNOWNMODE(nickname, channelname, mode) ": 472 " + nickname + " " + channelname + " " + mode + " :is not a recognised channel mode" + BREAK
#define ERR_INVITEONLYCHAN(nickname, channelname) (":localhost 473 " + nickname + " " + channelname + " :Cannot join channel (+i)" + BREAK)
#define ERR_BADCHANNELKEY(nickname, channelname) (": 475 " + nickname + " " + channelname + " :Bad channel key" + BREAK)
#define ERR_BADCHANNELMASK(nickname, channelname) (": 476 " + nickname + " " + channelname + " :Invalid channel name" + BREAK)
#define ERR_NOTOPERATOR(nickname, channelname) (":localhost 482 " + nickname + " " + channelname + " :You're not a channel operator" + BREAK)
// #define ERR_NOTOPERATOR(channelname) (":localhost 482 " + channelname + " :You're not a channel operator" + BREAK)
#define ERR_NEEDMODEPARM(channelname, mode) (": 696 " + channelname + " * You must specify a parameter for the key mode. " + mode + BREAK)
#define ERR_INVALIDMODEPARM(channelname, mode) ": 696 " + channelname + " Invalid mode parameter. " + mode + BREAK

enum messageCode {
	RPL_WELCOME = 1,
	RPL_CHANNELMODEIS = 324,
	RPL_CREATIONTIME = 329,
	RPL_TOPIC = 332,
	RPL_NOTOPIC = 331,
	RPL_TOPICWHOTIME = 333,
	RPL_NAMREPLY = 353,
	RPL_ENDOFNAMES = 366,
	RPL_TOPICADMIN,
	RPL_NICKCHANGE,
	RPL_JOINMSG,
	RPL_CREATECHANNELMSG,
	RPL_UMODEIS,
	RPL_CHANGEMODE,
	RPL_NICKNAMECHANGED,
	RPL_INVITING = 341,
	RPL_KICK,
 
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHCHANNEL = 403,
	ERR_TOOMANYCHANNELS = 405,
	ERR_UNKNOWCOMMAND = 421,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_ERRONEUSNICK = 432,
	ERR_NICKINUSE = 433,
	ERR_NOTONTHATCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_NOTREGISTERED = 451,
	ERR_NOTENOUGHPARAM = 461,
	ERR_ALREADYREGISTERED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_KEYSET = 467,
	ERR_CHANNELISFULL= 471,
	ERR_UNKNOWNMODE = 472,
	ERR_INVITEONLYCHAN = 473,
	ERR_BADCHANNELKEY = 475,
	ERR_BADCHANNELMASK = 476,
	ERR_CHANOPRIVSNEEDED  = 482,
	ERR_NEEDMODEPARM = 696,
	ERR_INVALIDMODEPARM,
};
