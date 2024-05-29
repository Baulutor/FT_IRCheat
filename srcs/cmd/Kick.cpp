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

void LFTarget(std::string & chanTarget, std::string & target, std::string & com, std::string cmd, Clients& client) {
    std::vector<std::string> splited = split(cmd, ' ');
    if (splited.size() == 1) {
        std::cout << "KICK <channel> <user> :<comment>" << std::endl;
        return(sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), ""), client));
    }
    chanTarget = splited[2];
    if (splited[1][0] == '#') {
        if (splited.size() == 2) {
            target = splited[2];
            com = "";
        }
        else if (splited.size() == 3) {
            if (splited[2][0] == '#') {
                target = splited[3].substr(1, splited[3].size());
                com = "";
            }
            else {
                target = splited[2];
                for (size_t i = 3; i != splited.size(); i++)
                    com += (splited[i] + " ");
            }
        }
        else {
            if (splited[2][0] == '#') {
                target = splited[3].substr(1, splited[3].size());
                for (size_t i = 4; i != splited.size(); i++) 
                    com += (splited[i] + " ");
            }
            else {
                target = splited[2];
                for (size_t i = 3; i != splited.size(); i++)
                    com += (splited[i] + " ");
            }
        }
    }
    else {
        if (splited.size() == 2) {
            std::cout << "KICK <channel> <user> :<comment>" << std::endl;
            return(sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), ""), client));
        }
        if (splited.size() == 3) {
            target = splited[3].substr(1, splited[3].size());
            com = "";
        }
        else {
            target = splited[3].substr(1, splited[3].size());
            for (size_t i = 4; i != splited.size(); i++) 
                com += (splited[i] + " ");
        }
    }
}


// void LFTarget(std::string &chanTarget, std::string &target, std::string &com, std::string cmd, Clients &client) {
//     std::vector<std::string> splited = split(cmd, ' ');
    
//     if (splited.size() < 3) {
//         std::cout << "KICK <channel> <user> :<comment>" << std::endl;
//         return sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), ""), client);
//     }
    
//     size_t targetIdx = 2;
//     size_t commentIdx = 3;
//     chanTarget = splited[2];

//     if (splited[1][0] == '#') {
//         if (splited.size() == 3 || (splited.size() > 3 && splited[2][0] == '#'))
//             targetIdx = 3;
//         if ((splited.size() > 3 && splited[2][0] == '#'))
//             commentIdx = 4;
//     } else {
//         if (splited.size() < 4) {
//             std::cout << "KICK <channel> <user> :<comment>" << std::endl;
//             return sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), ""), client);
//         }
//         if (splited.size() == 3)
//             targetIdx = 3;
//         commentIdx = 4;
//     }

//     target = splited[targetIdx][0] == '#' ? splited[targetIdx].substr(1) : splited[targetIdx];

//     com = "";
//     for (size_t i = commentIdx; i < splited.size(); i++) {
//         com += splited[i] + " ";
//     }
//     // if (!com.empty()) {
//     //     com.pop_back(); // Remove the trailing space
//     // }
// }


void Kick(std::string cmd, Clients& client, Server& server) {
    std::map<std::string, Channels>& channel = server.getChannels();
    std::map<std::string, Channels>::iterator it = channel.begin();
    std::string chanTarget;
    std::string target;
    std::string com;

    Clients op = it->second.getOperator(client.getNickname());
    if (op.getNickname() == "")
        return(sendCmd(ERR_NOTCHANOP(client.getNickname(), it->second.getName()), client));
    LFTarget(chanTarget, target, com, cmd, client);
    for (; it != channel.end(); it++) {
        if (it->first == chanTarget) {
            std::map<std::string, Clients>& clientsMap = it->second.getClientMap();
            std::map<std::string, Clients>::iterator it2 = clientsMap.begin();
            for(; it2 != clientsMap.end(); it2++) {
                if ((it2->first) == target) {
                    if (client.getNickname() == target)
                        return (sendBrodcastChannel(RPL_KICK_NOTICE(client.getNickname(), it->first), it->second));
                    sendBrodcastChannel(RPL_CMD_KICK(client.getNickname(), client.getUsername(), client.getAddrIp(), it->first, it2->first, com), it->second);
                    clientsMap.erase(it2);
                    return ;
                }
            }
            if (it2 == clientsMap.end())
                return (sendBrodcastChannel(ERR_USERNOTFOUND(client.getNickname(), target, it->first), it->second));
        }
    }
    if (it == channel.end()) {
        it--;
        return (sendBrodcastChannel(ERR_NOSUCHCHANNEL(client.getNickname(), chanTarget), it->second));
    }
}
