/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: afarachi <afarachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 23:51:14 by afarachi          #+#    #+#             */
/*   Updated: 2024/12/30 21:47:29 by afarachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cmd.hpp"

void Cmd::PASS(const Cmd& cmd, Server& server, Client& client) {
    std::cout << "Executing PASS command\n";
    // (void)cmd;
    (void)server;
    (void)client;
    std::string password = cmd.getParams()[0];
    
}