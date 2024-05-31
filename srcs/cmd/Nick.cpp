/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibernar <nibernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:15:55 by nibernar          #+#    #+#             */
/*   Updated: 2024/05/31 18:13:58 by nibernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"

/* 
    NICK <pseudo>
    regle a suivre pour le nickname
    - 9 char max
    - pas de string vide
    - pas de ',' ' ' '*' '?' '!' '@'
    - ne peu pas commencer par '#' '&'
    NICKLEN=9
*/

bool parseNick(std::string& nick, Clients& client) {
    if (nick.size() == 0 || nick.size() > 9) {
        sendCmd(RPL_ISUPPORT(client.getNickname(), "NICKEN=9"), client);
        return false;
    }
    for (size_t i = 0; i < nick.size(); i++) {
        if (nick[i] == ',' || nick[i] == ' ' || nick[i] == '*' || nick[i] == '?' || nick[i] == '!' || nick[i] == '@') {
            std::cerr << "ERROR char is not supported: " << nick[i] << std::endl;
            return false;
        }
    }
    if (nick[0] == '#' || nick[0] == '&') {
        std::cerr << "ERROR char is not supported: " << nick[0] << std::endl;
        return false;
    }
    return true;
}

void 	Nick(std::string cmd, Clients& client, Server& server) {
    std::map<int, Clients>& mapClients = server.getClients();
    std::map<int, Clients>::iterator itClients = mapClients.begin();
    std::vector<std::string> splited = split(cmd, ' ');
    if (splited.size() != 2) {
        sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), ""), client);
        return;
    }
    if (client.getNickname().size() != 0) {
        if (itClients->second.getNickname() == splited[1]) {
            sendCmd(RPL_ERROR_NICKNAME_IN_USE(client.getNickname(), itClients->second.getNickname()), client);
            return;
        }
    }
    if ( parseNick(splited[1], client) == true ) {
        //std::map<std::string, Channels>::iterator itChan = client.getChannels().begin();
        sendBrodcastServer(RPL_CMD_NICK(client.getNickname(), client.getUsername(), client.getAddrIp(), splited[1]), server);
        mapClients.find(client.getFd())->second.setNickname(splited[1]);
        client.setNickname(splited[1]);
        // for(; itChan != client.getChannels().end(); itChan++) {
        //     std::cout << "chan ==== " << itChan->first << std::endl;
        //     NameLstUpadte(client, itChan->second);
        // }
    }
}