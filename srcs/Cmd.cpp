/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moabbas <moabbas@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:11:08 by afarachi          #+#    #+#             */
/*   Updated: 2025/01/08 13:06:59 by moabbas          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"
#include <sstream>
#include <algorithm>
#include <iostream>
#include "../includes/Cmd.hpp"
#include "../includes/Errors.hpp"

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
    _commands["KICK"] = &Cmd::KICK;
    _commands["INVITE"] = &Cmd::INVITE;
    _commands["TOPIC"] = &Cmd::TOPIC;
    _commands["MODE"] = &Cmd::MODE;
    _commands["QUIT"] = &Cmd::QUIT;
}

Cmd::Cmd(const Cmd& other) {
    *this = other;
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

void Cmd::execute(Server& server ,Client& client) const
{
    // std::cout << "New command: " << _cmdName << std::endl;
    // if(!_cmdParams.empty())
    // {
    //     std::cout << "Parameters: ";
    //     for(size_t i = 0; i < _cmdParams.size(); ++i)
    //     {
    //         std::cout << _cmdParams[i];
    //         if(i < _cmdParams.size() - 1)
    //             std::cout << ", ";
    //     }
    //     std::cout << std::endl;
    // }

    std::map<std::string ,CommandCallback>::const_iterator it = _commands.find(_cmdName);
    if(it != _commands.end())
        it->second(*this ,server ,client);
}

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

void Cmd::errorServerClient(std::string s_side, std::string c_side, int c_fd) {
    if (s_side != "") std::cerr << s_side << std::endl;
    send(c_fd, c_side.c_str(), c_side.size(), 0);
}

void Parser::parse(std::list<Cmd> *commandsList, std::string input, Client& client, Server &server) {
    std::list<Cmd> commands = Parser::splitCommands(input, client, server);
    commandsList->splice(commandsList->end(), commands);
}

std::list<Cmd> Parser::splitCommands(std::string input, Client& client, Server &server) {
    size_t start = 0, end;
    std::list<Cmd> result;
    while ((end = input.find('\n', start)) != std::string::npos) {
        std::string command = input.substr(start, end - start);
        command.push_back('\n');
        Cmd parsedCommand = parseCommand(command);
        if (!Errors::commandFound(parsedCommand.getName())) {
            start = end + 1;
            if (parsedCommand.getName().empty() ||(parsedCommand.getName() != "WHO" && parsedCommand.getName() != "CAP")) {
                std::string messageArgs[] = {client.getNickname(), parsedCommand.getName()};
                Server::sendError(messageArgs, client.getFd(), ERR_UNKNOWCOMMAND);
            }
            continue;
        }
        if (Errors::commandFound(parsedCommand.getName()) && !Errors::validParameters(parsedCommand, client, server)) {
            start = end + 1;
            continue;
        }
        result.push_back(parsedCommand);
        start = end + 1;
    }
    return result;
}

Cmd Parser::parseCommand(std::string clientBuffer) {
    size_t pos;
    std::string commandName;
    std::vector<std::string> params;
    while((pos = clientBuffer.find('\n')) != std::string::npos) {
        std::string command = clientBuffer.substr(0 ,pos);
        clientBuffer.erase(0 ,pos + 1);
        if(!command.empty() && command[command.size() - 1] == '\r')
            command.erase(command.size() - 1);
        std::string trimmed = trimString(command);
        if(trimmed.empty() || trimmed == "CAP LS 302" || trimmed == "CAP LS")
            continue;
        size_t firstSpace = trimmed.find(' ');
        commandName = trimmed.substr(0 ,firstSpace);
        if(firstSpace != std::string::npos) {
            size_t startIdx = 0 ,endIdx;
            std::string rest = trimString(trimmed.substr(firstSpace + 1));
            while((endIdx = rest.find(' ' ,startIdx)) != std::string::npos) {
                std::string param = rest.substr(startIdx ,endIdx - startIdx);
                if(!param.empty() && param[0] == ':') {
                    params.push_back(rest.substr(startIdx + 1));
                    startIdx = rest.size();
                    break;
                }
                params.push_back(param);
                while(endIdx != rest.size() && isspace(rest[endIdx]))
                    endIdx++;
                startIdx = endIdx;
            }
            if(startIdx < rest.size()) {
                std::string lastParam = rest.substr(startIdx);
                if (lastParam.size() == 2 && lastParam == "::")
                    params.push_back("");
                else if(!lastParam.empty() && lastParam[0] == ':')
                    params.push_back(lastParam.substr(1));
                else
                    params.push_back(lastParam);
            }
        } else
            commandName = trimmed;
    }
    return Cmd(commandName ,params);
}
