/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRIVMSG.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:48:58 by afarachi          #+#    #+#             */
/*   Updated: 2024/12/28 14:55:03 by afarachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"

void Cmd::PRIVMSG(const Cmd& cmd, Server& server, Client& client) {
    std::cout << "Executing PRIVMSG command\n";
    (void)cmd;
    (void)server;
    (void)client;
    // @mabbas && @jfatfat > here we can  implement PRIVMSG
}