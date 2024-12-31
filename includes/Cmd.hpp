/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:10:02 by afarachi          #+#    #+#             */
/*   Updated: 2024/12/31 13:47:17 by moabbas          ###   ########.fr       */
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
    static std::map<std::string, CommandCallback> _commands; // map 3al cmdsssss

public:
    Cmd(const Cmd& other);
    Cmd& operator=(const Cmd& other);
    ~Cmd();

    std::string getName() const;
    const std::vector<std::string>& getParams() const;

    void execute(Server& server, Client& client) const;

    //  Commands
    static void PASS(const Cmd& cmd, Server& server, Client& client);
    static void NICK(const Cmd& cmd, Server& server, Client& client);
    static void USER(const Cmd& cmd, Server& server, Client& client);
    static void PING(const Cmd& cmd, Server& server, Client& client);
    static void PRIVMSG(const Cmd& cmd, Server& server, Client& client);
    static void JOIN(const Cmd& cmd, Server& client, Client& server);
    static void PART(const Cmd& cmd, Server& client, Client& server);
    
    Cmd(const std::string& name, const std::vector<std::string>& params);
    static void errorServerClient(std::string s_side, std::string c_side, int c_fd);
};

class Parser {
    private:
        static Cmd parseCommand(std::string input);
        static std::list<Cmd> splitCommands(std::string input, Client& client, Server &server);
        
    public:
        static void parse(std::list<Cmd> *commandsList, std::string input, Client& client, Server &server);
        
};

#endif
