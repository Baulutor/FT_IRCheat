/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:14:18 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/13 16:37:36 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"
#include "Channels.hpp"

//KICK <channel> <user> :<comment>
//peu t on kick un operator ?
void Kick(std::string cmd, Clients& client, Server& server) {
    std::map<std::string, Channels>& channel = server.getChannels();
    std::map<std::string, Channels>::iterator it = channel.begin();
    std::vector<std::string> splited = splitEOF(cmd, ' ');
    std::string cible;
    std::string com;

    if (splited.size() == 2) {
        std::cout << "KICK <channel> <user> :<comment>" << std::endl;
        return(sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), ""), client));
    }
    if (splited.size() == 3) {
        std::cout << "KICK <channel> <user> :<comment>" << std::endl;
        return(sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), ""), client));
    }
    std::vector<Clients> clientsOperator = it->second.getOperator();
    std::vector<Clients>::iterator itOp = clientsOperator.begin();
    for (; itOp != clientsOperator.end(); itOp++) {
        if (itOp->getNickname() == client.getNickname())
            break;
    }
    if (itOp == clientsOperator.end())
        return(sendCmd(ERR_NOTCHANOP(client.getNickname(), it->second.getName()), client));
    if (splited[4].size() != 0) {
        cible = splited[3];
        for (size_t i = 4; i != splited.size(); i++) 
            com += (splited[i] + " ");
    }
    else {
        com = "";
        cible = splited[3].substr(0, splited[3].size() - 2);
    }

    for (; it != channel.end(); it++) {
        if (it->first == splited[2]) {
            std::map<std::string, Clients>& clientsMap = it->second.getClientMap();
            std::map<std::string, Clients>::iterator it2 = clientsMap.begin();
            for(; it2 != clientsMap.end(); it2++) {
                std::cout << "couille ici = " << it2->first << "fd = " << it2->second.getFd() << std::endl;
                if ((':' + it2->first) == cible) {
                    if (':' + client.getNickname() == cible)
                        return (sendBrodcastChannel("connot kick yourself\n\r", it->second));
                    sendBrodcastChannel(RPL_CMD_KICK(client.getNickname(), client.getUsername(), client.getAddrIp(), it->first, it2->first, com), it->second);
                    clientsMap.erase(it2);
                    return ;
                }
            }
            if (it2 == clientsMap.end())
                return (sendBrodcastChannel(ERR_USERNOTFOUND(client.getNickname(), cible, it->first), it->second));
        }
    }
    if (it == channel.end()) {
        it--;
        return (sendBrodcastChannel(ERR_NOSUCHCHANNEL(client.getNickname(), splited[2]), it->second));
    }
}
