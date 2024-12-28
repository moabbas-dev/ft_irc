/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:50:06 by afarachi          #+#    #+#             */
/*   Updated: 2024/12/28 14:55:09 by afarachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"

void Cmd::USER(const Cmd& cmd, Server& server, Client& client) {
    std::cout << "Executing USER command\n";
    (void)cmd;
    (void)server;
    (void)client;
    // @mabbas && @jfatfat > here we can  implement USER
}