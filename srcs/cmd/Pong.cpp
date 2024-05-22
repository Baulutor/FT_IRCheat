/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibernar <nibernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 11:52:10 by nibernar          #+#    #+#             */
/*   Updated: 2024/05/22 13:36:40 by nibernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"

void Pong(std::string cmd, Clients& client, Server& server)
{
    (void) server;
    std::cout << "PONG" << std::endl;
    std::vector<std::string> splited = split(cmd, ' ');
    if (splited.size() < 2) {
        //envoyer le bon broadcast ici a la place de error arg
        std::cerr << "error arg" << std::endl;
        return ;
    }
    std::cout << "pong : " << RPL_CMD_PONG(client.getNickname(), client.getUsername(), client.getAddrIp(), splited[1]) << std::endl;
    sendCmd(RPL_CMD_PONG(client.getNickname(), client.getUsername(), client.getAddrIp(), splited[1]), client);
}