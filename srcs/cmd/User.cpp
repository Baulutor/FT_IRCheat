/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibernar <nibernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:15:48 by nibernar          #+#    #+#             */
/*   Updated: 2024/05/30 18:43:49 by nibernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"
/*    regle a suivre pour le nickname
    - pas de string vide
    - pas de ',' ' ' '*' '?' '!' '@'
    - ne peu pas commencer par '#' '&' 
*/

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
    (void) server;
    std::vector<std::string> splited = split(cmd, ' ');
    std::cout << "USER = " << cmd << std::endl;
    for (size_t i = 0; i < splited.size(); i++)
        std::cout << "splited [" << i << "]" << " = " << splited[i] << "|" << std::endl;
    // check le rpl si on a plus de 1 param
    if (splited.size() != 1) {
        sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), ""), client);
        return;
    }
    if (client.getNickname().size() != 0) {
        //chercher les user dans les server et changer de rpl ERR_NICKNAMEINUSE (433) "<client> <nick> :Nickname is already in use"
        sendCmd(ERR_ALREADYREGISTERED(client.getNickname()), client);
        return;       
    }
    if ( parseUser(splited[1], client) == true )
        client.setUsername(splited[1]);
}