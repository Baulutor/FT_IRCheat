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
    std::vector<std::string> splited = split(cmd, ' ');
    std::string reponse;
    std::map<std::string, Channels> channel = server.getChannels();
    std::cout << "client name = " << client.getNickname() << std::endl;
    // std::cout << "cible = " << splited[3] << std::endl;
    if (splited.size() == 2) {
        return(sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), ""), client));
    }
    if (splited.size() == 3) {
        return(sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), ""), client));
    }
    //check si l'operateur de la cmd kick existe, si il a les droits de kick
    //si il n'a pas les droits de kick ERR_CHANOPRIVSNEEDED
    for (std::map<std::string, Channels>::iterator it = channel.begin(); it != channel.end(); it++) {
        std::cout << "channel name = " << it->first << std::endl;
        if (it->first == splited[1]) {
            //if (it->first != splited[1]) ERR_NOSUCHCHANNE
            std::cout << "chan trouv2 " << it->first << std::endl;
            std::map<std::string, Clients> clientsMap = it->second.getClientMap();
            for(std::map<std::string, Clients>::iterator it2 = clientsMap.begin() ; it2 != clientsMap.end(); it2++) {
                std::cout << "client in the channel = " << it2->second.getNickname() << std::endl;
                std::cout << "client in the channel = " << it2->first << std::endl;
                std::cout << "splited[3] = " << splited[3] << std::endl;
                if ((':' + it2->first) == splited[3]) {
                    //if ((':' + it2->first) != splited[3]) ERR_USERNOTINCHANNEL
                    std::cout << "client a kick trouv2" << std::endl;
                    reponse = ":" + client.getNickname() + " KICK " + splited[2] + " " + splited[3] + " :" + splited[4] + "\r\n"; 
                    if (send(client.getFd(), reponse.c_str(), reponse.size(), 0) == -1)
                        throw (SendErrorExeption());
                }
            }
        }
    }
}