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
// on doit pourvoir kick le user a partir du channel d'ou on fait la cmd, on peu kick un seul chan et une seul persoone a la fois
void Kick(std::string cmd, Clients& client, Server& server) {
    std::cout << "KICK" << std::endl;

    std::map<std::string, Channels>& channel = server.getChannels();
    std::vector<std::string> splited = split(cmd, ' ');
    std::string cible;
    std::string com;

    if (splited.size() == 2) {
        return(sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), ""), client));
    }
    if (splited.size() == 3) {
        return(sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), ""), client));
    }
    if (splited[4].size() != 0) {
        cible = splited[3];
        for (size_t i = 4; i != splited.size(); i++) 
            com += (splited[i] + " ");
    }
    else {
        com = "";
        cible = splited[3].substr(0, splited[3].size() - 2);
    }

    //si il n'a pas les droits de kick ERR_CHANOPRIVSNEEDED
    std::map<std::string, Channels>::iterator it = channel.begin();
    for (; it != channel.end(); it++) {
        if (it->first == splited[2]) {
            std::map<std::string, Clients>& clientsMap = it->second.getClientMap();
            std::map<std::string, Clients>::iterator it2 = clientsMap.begin();
            for(; it2 != clientsMap.end(); it2++) {
                if ((':' + it2->first) == cible) {
                    if (':' + client.getNickname() == cible) {
                        std::cout << "tu peu pas t'autockick trou d'uc" << std::endl;
                        return;
                    }
                    clientsMap.erase(it2);
                    NameLstUpadte(client, it->second);
                    sendBrodcastChannel(RPL_CMD_KICK(client.getNickname(), client.getUsername(), client.getAddrIp(), it->first, cible, com), it->second);
                    return ;
                }
            }
            if (it2 == clientsMap.end()) {
                //ERR_USERNOTINCHANNEL
                std::cout << "client pas trouve" << std::endl;
                return ;
            }
        }
    }
    if (it == channel.end()) {
        //ERR_NOSUCHCHANNE
        std::cout << "chanel pas trouve" << std::endl;
    }
}
