/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:11:08 by afarachi          #+#    #+#             */
/*   Updated: 2024/12/28 16:40:47 by afarachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"
#include <sstream>
#include <algorithm>
#include <iostream>
#include "../includes/Cmd.hpp"


std::map<std::string, Cmd::CommandCallback> Cmd::_commands;

Cmd::Cmd(const std::string &name, const std::vector<std::string> &params)
    : _cmdName(name), _cmdParams(params) {}

Cmd::Cmd(const Cmd &other) : _cmdName(other._cmdName), _cmdParams(other._cmdParams) {}

Cmd &Cmd::operator=(const Cmd &other)
{
    if (this != &other)
    {
        _cmdName = other._cmdName;
        _cmdParams = other._cmdParams;
    }
    return *this;
}

Cmd::~Cmd() {}

std::string Cmd::getName() const
{
    return _cmdName;
}

const std::vector<std::string> &Cmd::getParams() const
{
    return _cmdParams;
}

// Register all commands in the static map
// void Cmd::registerCommands() {
//     _commands["PASS"] = &Cmd::PASS;
//     _commands["NICK"] = &Cmd::NICK;
//     _commands["USER"] = &Cmd::USER;
//     _commands["PING"] = &Cmd::PING;
//     _commands["PRIVMSG"] = &Cmd::PRIVMSG;
//     _commands["JOIN"] = &Cmd::JOIN;
//     _commands["PART"] = &Cmd::PART;
// }

// ⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️ Execute the command by finding the callback in the map ⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️
// void Cmd::execute(Server& server, Client& client) const {
//     std::map<std::string, CommandCallback>::const_iterator it = _commands.find(_cmdName);
//     if (it != _commands.end()) {
//         it->second(*this, server, client); //⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️ Call the callback function ⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️⚠️
//     } else {
//         std::cerr << "Error: Unknown command: " << _cmdName << std::endl;
//     }
// }

// enso yalli fo2 kenet 3amm jarreb 3layhon

void Cmd::execute(Server &server, Client &client) const
{
    // Log the command and its parameters for tracking
    std::cout << "Executing command: " << _cmdName << std::endl;
    if (!_cmdParams.empty())
    {
        std::cout << "Parameters: ";
        for (size_t i = 0; i < _cmdParams.size(); ++i)
        {
            std::cout << _cmdParams[i];
            if (i < _cmdParams.size() - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }

    std::map<std::string, CommandCallback> _commands = {
        {"JOIN", &Cmd::JOIN},
        {"NICK", &Cmd::NICK},
        {"USER", &Cmd::PART},
        {"PASS", &Cmd::PASS},
        {"PING", &Cmd::PING},
        {"USER", &Cmd::USER},
        {"PRIVMSG", &Cmd::PRIVMSG}};
    // Search for the command in the map
    std::map<std::string, CommandCallback>::const_iterator it = _commands.find(_cmdName);
    if (it != _commands.end())
    {
        // Execute the command using the corresponding callback
        it->second(*this, server, client);
    }
    else
    {
        // Handle unknown commands
        std::cerr << "Error: Unknown command: " << _cmdName << std::endl;
        // Optionally, you can send an error message back to the client
        std::string errorMessage = "Error: Unknown command: " + _cmdName + "\n";
        send(client.getFd(), errorMessage.c_str(), errorMessage.size(), 0);
    }
}


// 🤔🧩 maybe we will use it later !!

// Cmd Cmd::parseClientCommand(const std::string &input)
// {
//     std::istringstream iss(input);
//     std::string name;
//     std::vector<std::string> params;

//     iss >> name;

//     std::string param;
//     while (iss >> param)
//     {
//         if (param[0] == ':')
//         {
//             std::string restOfMessage;
//             getline(iss, restOfMessage);
//             params.push_back(param.substr(1) + restOfMessage);
//             break;
//         }
//         params.push_back(param);
//     }
//     return Cmd(name, params);
// }
