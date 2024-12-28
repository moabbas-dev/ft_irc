/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:10:02 by afarachi          #+#    #+#             */
/*   Updated: 2024/12/28 14:56:55 by afarachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_HPP
#define CMD_HPP

#include <string>
#include <vector>
#include <map>
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

class Cmd {
public:
    typedef void (*CommandCallback)(const Cmd&, Server&, Client&); // sho ra2ykon bhal  fekra : :P ⚠️⚠️⚠️⚠️⚠️ s2alt chat GPT w alli mni7a bass ma t3ida⚠️⚠️⚠️⚠️⚠️⚠️

private:
    std::string _cmdName;
    std::vector<std::string> _cmdParams;
    static std::map<std::string, CommandCallback> _commands; // map 3al cmdsssss

public:
    Cmd(const std::string& name, const std::vector<std::string>& params);
    Cmd(const Cmd& other);
    Cmd& operator=(const Cmd& other);
    ~Cmd();

    std::string getName() const;
    const std::vector<std::string>& getParams() const;

    void execute(Server& server, Client& client) const;

    //  Commandsssssssssssssssssssssss 🧩
    static void PASS(const Cmd& cmd, Server& server, Client& client);
    static void NICK(const Cmd& cmd, Server& server, Client& client);
    static void USER(const Cmd& cmd, Server& server, Client& client);
    static void PING(const Cmd& cmd, Server& server, Client& client);
    static void PRIVMSG(const Cmd& cmd, Server& server, Client& client);
    static void JOIN(const Cmd& cmd, Server& client, Client& server);
    static void PART(const Cmd& cmd, Server& client, Client& server);

    // 🤔
    static void registerCommands();
};

#endif
