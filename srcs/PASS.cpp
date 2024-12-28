/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:51:14 by afarachi          #+#    #+#             */
/*   Updated: 2024/12/28 14:54:52 by afarachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"

void Cmd::PASS(const Cmd& cmd, Server& server, Client& client) {
    std::cout << "Executing PASS command\n";
    (void)cmd;
    (void)server;
    (void)client;
    // @mabbas && @jfatfat > here we can  implement PASS
}