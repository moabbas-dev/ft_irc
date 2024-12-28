/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:50:34 by afarachi          #+#    #+#             */
/*   Updated: 2024/12/28 14:54:35 by afarachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"

void Cmd::NICK(const Cmd& cmd, Server& server, Client& client) {
    std::cout << "Executing NICK command\n";
    (void)cmd;
    (void)server;
    (void)client;
    // @mabbas && @jfatfat > here we can  implement NICK
}