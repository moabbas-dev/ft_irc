/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:11:08 by afarachi          #+#    #+#             */
/*   Updated: 2024/12/29 12:04:59 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"
#include <sstream>
#include <algorithm>
#include <iostream>
#include "../includes/Cmd.hpp"


std::map<std::string ,Cmd::CommandCallback> Cmd::_commands;

Cmd::Cmd(const std::string& name ,const std::vector<std::string>& params)
    : _cmdName(name) ,_cmdParams(params) {
    _commands["JOIN"] = &Cmd::JOIN;
    _commands["NICK"] = &Cmd::NICK;
    _commands["PART"] = &Cmd::PART;
    _commands["PASS"] = &Cmd::PASS;
    _commands["PING"] = &Cmd::PING;
    _commands["USER"] = &Cmd::USER;
    _commands["PRIVMSG"] = &Cmd::PRIVMSG;
}

Cmd::Cmd(const Cmd& other): _cmdName(other._cmdName) ,_cmdParams(other._cmdParams) {

}

Cmd& Cmd::operator=(const Cmd& other)
{
    if(this != &other)
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

const std::vector<std::string>& Cmd::getParams() const
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

void Cmd::execute(Server& server ,Client& client) const
{
    // Log the command and its parameters for tracking
    std::cout << "New command: " << _cmdName << std::endl;
    if(!_cmdParams.empty())
    {
        std::cout << "Parameters: ";
        for(size_t i = 0; i < _cmdParams.size(); ++i)
        {
            std::cout << _cmdParams[i];
            if(i < _cmdParams.size() - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;
    }

    // std::map<std::string, CommandCallback> _commands = {
    //     {"JOIN", &Cmd::JOIN},
    //     {"NICK", &Cmd::NICK},
    //     {"PART", &Cmd::PART},
    //     {"PASS", &Cmd::PASS},
    //     {"PING", &Cmd::PING},
    //     {"USER", &Cmd::USER},
    //     {"PRIVMSG", &Cmd::PRIVMSG}
    // };
    // Search for the command in the map
    std::map<std::string ,CommandCallback>::const_iterator it = _commands.find(_cmdName);
    if(it != _commands.end())
        // Execute the command using the corresponding callback
        it->second(*this ,server ,client);
    else
    {
        // Handle unknown commands
        std::cerr << "Error: Unknown command: " << _cmdName << std::endl;
        // Optionally, you can send an error message back to the client
        std::string errorMessage = "Error: Unknown command: " + _cmdName + "\n";
        send(client.getFd() ,errorMessage.c_str() ,errorMessage.size() ,0);
    }
}

// 🤔🧩 maybe we will use it later !!
// Yes maybe, but why do u perfer to put emojies? they are not serious as far as i can tell :)
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

std::string trimString(const std::string& str)
{
    std::string::size_type start = 0;
    while(start < str.length() && std::isspace(str[start]))
        ++start;

    std::string::size_type end = str.length();
    while(end > start && std::isspace(str[end - 1]))
        --end;

    return str.substr(start ,end - start);
}

Cmd Cmd::parseClientCommand(std::string clientBuffer) {
    // Process complete commands
    size_t pos;
    std::string commandName;
    std::vector<std::string> params;
    while((pos = clientBuffer.find('\n')) != std::string::npos) {
        std::string command = clientBuffer.substr(0 ,pos);
        clientBuffer.erase(0 ,pos + 1); // Remove processed command from buffer
        if(!command.empty() && command[command.size() - 1] == '\r')
            command.erase(command.size() - 1);// Remove \r if present
        std::string trimmed = trimString(command);
        if(trimmed.empty() || trimmed == "CAP LS")
            continue;

        // Parse command
        size_t firstSpace = trimmed.find(' ');
        commandName = trimmed.substr(0 ,firstSpace);
        if(firstSpace != std::string::npos) {
            std::string rest = trimmed.substr(firstSpace + 1);
            size_t startIdx = 0 ,endIdx;
            while((endIdx = rest.find(' ' ,startIdx)) != std::string::npos) {
                std::string param = rest.substr(startIdx ,endIdx - startIdx);
                if(!param.empty() && param[0] == ':') {
                    // Treat everything after ':' as a single parameter
                    // example: when we have this command: PRIVMSG #channel :This is a message
                    // => so there are 2 params in this case: #channel and (This is a message)
                    params.push_back(rest.substr(startIdx + 1));
                    startIdx = rest.size();
                    break; // Stop splitting but continue processing the function
                }
                params.push_back(param);
                startIdx = endIdx + 1;
            }

            // Handle the last parameter (in case no ':' was found earlier)
            if(startIdx < rest.size()) {
                std::string lastParam = rest.substr(startIdx);
                if(!lastParam.empty() && lastParam[0] == ':')
                    params.push_back(lastParam.substr(1));
                else
                    params.push_back(lastParam);
            }
        } else
            commandName = trimmed;
    }
    Cmd cmd(commandName ,params);
    return cmd;
}
