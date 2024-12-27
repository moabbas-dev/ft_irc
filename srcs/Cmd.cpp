/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:11:08 by afarachi          #+#    #+#             */
/*   Updated: 2024/12/27 23:52:30 by afarachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"
#include <sstream>
#include <algorithm>
#include <iostream>

std::map<std::string, Cmd::CommandCallback> Cmd::_commands;

Cmd::Cmd(const std::string& name, const std::vector<std::string>& params)
    : _name(name), _params(params) {}

Cmd::Cmd(const Cmd& other) : _name(other._name), _params(other._params) {}

Cmd& Cmd::operator=(const Cmd& other) {
    if (this != &other) {
        _name = other._name;
        _params = other._params;
    }
    return *this;
}

Cmd::~Cmd() {}

std::string Cmd::getName() const {
    return _name;
}

const std::vector<std::string>& Cmd::getParams() const {
    return _params;
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
    std::map<std::string, CommandCallback>::const_iterator it = _commands.find(_name);
    if (it != _commands.end()) {
        it->second(*this, server, client); //⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️ Call the callback function ⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️
    } else {
        std::cerr << "Error: Unknown command: " << _name << std::endl;
    }
}












