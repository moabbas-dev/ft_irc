/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:11:08 by afarachi          #+#    #+#             */
/*   Updated: 2024/12/28 14:58:14 by afarachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"
#include <sstream>
#include <algorithm>
#include <iostream>

std::map<std::string, Cmd::CommandCallback> Cmd::_commands;

Cmd::Cmd(const std::string& name, const std::vector<std::string>& params)
    : _cmdName(name), _cmdParams(params) {}

Cmd::Cmd(const Cmd& other) : _cmdName(other._cmdName), _cmdParams(other._cmdParams) {}

Cmd& Cmd::operator=(const Cmd& other) {
    if (this != &other) {
        _cmdName = other._cmdName;
        _cmdParams = other._cmdParams;
    }
    return *this;
}

Cmd::~Cmd() {}

std::string Cmd::getName() const {
    return _cmdName;
}

const std::vector<std::string>& Cmd::getParams() const {
    return _cmdParams;
}

// Register all commands in the static map
void Cmd::registerCommands() {
    _commands["PASS"] = &Cmd::PASS;
    _commands["NICK"] = &Cmd::NICK;
    _commands["USER"] = &Cmd::USER;
    _commands["PING"] = &Cmd::PING;
    _commands["PRIVMSG"] = &Cmd::PRIVMSG;
    _commands["JOIN"] = &Cmd::JOIN;
    _commands["PART"] = &Cmd::PART;
}

// ⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️ Execute the command by finding the callback in the map ⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️
void Cmd::execute(Server& server, Client& client) const {
    std::map<std::string, CommandCallback>::const_iterator it = _commands.find(_cmdName);
    if (it != _commands.end()) {
        it->second(*this, server, client); //⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️ Call the callback function ⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️
    } else {
        std::cerr << "Error: Unknown command: " << _cmdName << std::endl;
    }
}












