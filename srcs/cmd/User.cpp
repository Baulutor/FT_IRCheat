/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibernar <nibernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:15:48 by nibernar          #+#    #+#             */
/*   Updated: 2024/05/31 14:07:21 by nibernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"

bool parseUser(std::string& user, Clients& client) {
    if (user.size() == 0 || user.size() > 12) {
        sendCmd(RPL_ISUPPORT(client.getNickname(), "USERLEN=12"), client);
        return false;
    }
    for (size_t i = 0; i < user.size(); i++) {
        if (user[i] == ',' || user[i] == ' ' || user[i] == '*' || user[i] == '?' || user[i] == '!' || user[i] == '@') {
            std::cerr << "ERROR char is not supported: " << user[i] << std::endl;
            return false;
        }
    }
    if (user[0] == '#' || user[0] == '&') {
        std::cerr << "ERROR char is not supported: " << user[0] << std::endl;
        return false;
    }
    return true;
}

void 	User(std::string cmd, Clients& client, Server& server) {
    std::map<int, Clients>& mapClients = server.getClients();
    std::map<int, Clients>::iterator itClients = mapClients.begin();
    std::vector<std::string> splited = split(cmd, ' ');
    if (splited.size() != 2) {
        sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), ""), client);
        return;
    }
    if (client.getUsername().size() != 0) {
        if (itClients->second.getUsername() == splited[1]) {
            sendCmd(RPL_ERROR_NICKNAME_IN_USE(client.getUsername(), itClients->second.getUsername()), client);
            return;
        }
    }
    if ( parseUser(splited[1], client) == true ) {
        client.setUsername(splited[1]);
        //sendCmd(RPL_CMD_NICK(falseNickname, getUsername(), getAddrIp(), tokens[i + 1]), client);
    }
}