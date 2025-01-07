/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:10:02 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/05 21:55:05 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_HPP
#define CMD_HPP

#include <string>
#include <vector>
#include <list>
#include <map>
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

#define _USERLEN 10

class Server;
class Client;

class Cmd {
public:
    typedef void (*CommandCallback)(const Cmd&, Server&, Client&);

private:
    std::string _cmdName;
    std::vector<std::string> _cmdParams;
    static std::map<std::string, CommandCallback> _commands;

public:
    Cmd(const Cmd& other);
    Cmd(const std::string& name, const std::vector<std::string>& params);
    Cmd& operator=(const Cmd& other);
    ~Cmd();

    std::string getName() const;
    const std::vector<std::string>& getParams() const;


    //  Commands
    static void PASS(const Cmd& cmd, Server& server, Client& client);
    static void NICK(const Cmd& cmd, Server& server, Client& client);
    static void USER(const Cmd& cmd, Server& server, Client& client);
    static void PING(const Cmd& cmd, Server& server, Client& client);
    static void PRIVMSG(const Cmd& cmd, Server& server, Client& client);
    static void JOIN(const Cmd& cmd, Server& client, Client& server);
    static void PART(const Cmd& cmd, Server& client, Client& server);
    static void KICK(const Cmd& cmd, Server& server, Client& client);
    static void INVITE(const Cmd& cmd, Server& server, Client& client);
    static void TOPIC(const Cmd& cmd, Server& server, Client& client);
    static void MODE(const Cmd& cmd, Server& server, Client& client);
    static void QUIT(const Cmd& cmd, Server& server, Client& client);
    
    // execute command
    void execute(Server& server, Client& client) const;
    
    // send the first message to the server and the second to the client
    static void errorServerClient(std::string s_side, std::string c_side, int c_fd);
};

class Parser {
    public:
        // parse the raw command in the server
        static void parse(std::list<Cmd> *commandsList, std::string input, Client& client, Server &server);

    private:
        // split the commands if the message contains more than one separated with "\r\n" or "\n"
        static std::list<Cmd> splitCommands(std::string input, Client& client, Server &server);

        // parse commands one by one
        static Cmd parseCommand(std::string input);
};

#endif
