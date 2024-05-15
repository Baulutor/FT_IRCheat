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
/*
:nickname!username@host KICK #channel user :comment
Où nickname!username@host représente l'utilisateur qui a initié la commande KICK.
*/
//recup class serveur !
void Kick(std::string cmd, Clients& client, std::map<std::string, Channels>& channelsServer) {
    std::cout << "KICK" << std::endl;
    std::vector<std::string> splited = split(cmd, ' ');
    std::string reponse;
    // for (std::vector<std::string>::iterator it = splited.begin(); it != splited.end(); it++)
    //     std::cout << "splited = " << *it << '|' << std::endl;
    // std::cout << "cmd = " << cmd << std::endl;
    // std::cout << "client name = " << client.getNickname() << std::endl;
    for (std::map<std::string, Channels>::iterator it = channelsServer.begin(); it != channelsServer.end(); it++) {
        std::cout << it->first << std::endl;
        Channels & channelTmp = it->second;
        std::map<int, Clients> clientTmp = channelTmp.getClientMap();
        for (std::map<int, Clients>::iterator it2 = clientTmp.begin(); it2 != clientTmp.end(); it2++) {
            std::cout << "caca" << it2->first << std::endl;
        }
    }
    if (('#' + splited[1]) == splited[2]) {
        // changer ':' + client.getNickname() par un find de std::map<int, Clients> _clients;
        if (':' + client.getNickname() == splited[3]) {
            reponse = ":" + client.getNickname() + " KICK " + splited[2] + " " + splited[3] + " :" + splited[4] + "\r\n"; 
            if (send(client.getFd(), reponse.c_str(), reponse.size(), 0) == -1)
                throw (SendErrorExeption());
        }
    }
}