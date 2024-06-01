/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibernar <nibernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:15:55 by nibernar          #+#    #+#             */
/*   Updated: 2024/06/01 15:03:22 by nibernar         ###   ########.fr       */
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

bool parseNick(std::string& nick, Clients& client)
{
    std::cout << "parseNick" << std::endl;
    if (nick.size() == 0 || nick.size() > 30)
    {
        std::cout << "RPL_ISUPPORT = " << RPL_ISUPPORT(client.getNickname(), "NICKLEN=30") << std::endl;
        sendCmd(RPL_ISUPPORT(client.getNickname(), "NICKLEN=30"), client);
        return (false);
    }
    for (size_t i = 0; i < nick.size(); i++) {
        if (nick[i] == ',' || nick[i] == ' ' || nick[i] == '*' || nick[i] == '?' || nick[i] == '!' || nick[i] == '@' || (nick[i] >= 9 && nick[i] <= 13))
        {
            std::cout << "ERR_ERRONEUSNICKNAME = " << ERR_ERRONEUSNICKNAME(client.getNickname(), nick) << std::endl;
            sendCmd(ERR_ERRONEUSNICKNAME(client.getNickname(), nick), client);
            return (false);
        }
    }
    if (nick[0] == '#' || nick[0] == '&' || nick[0] == ':')
    {
        std::cout << "ERR_ERRONEUSNICKNAME = " << ERR_ERRONEUSNICKNAME(client.getNickname(), nick) << std::endl;
        sendCmd(ERR_ERRONEUSNICKNAME(client.getNickname(), nick), client);
        return (false);
    }
    return (true);
}

void 	Nick(std::string cmd, Clients& client, Server& server)
{
    std::cout << "Nick" << cmd << std::endl;
    static bool nickname_used = false;
    static std::string falseNickname = "";
    std::map<int, Clients>& mapClients = server.getClients();
    // std::map<int, Clients>::iterator itClients = mapClients.begin();
    std::vector<std::string> splited = split(cmd, ' ');
    if (splited.size() != 2)
    {
        std::cout << "ERR_NEEDMOREPARAMS = " << ERR_NEEDMOREPARAMS(client.getNickname(), "NICK") << std::endl;
        return (sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), "NICK"), client));
    }
    if (parseNick(splited[1], client) == false)
        return ;
    if (client.getNickname().size() != 0)
    {
        for (std::map<int, Clients>::iterator it = server.getClients().begin(); it != server.getClients().end(); it++)
        {
            if (it->second.getNickname() == splited[1])
            {
                nickname_used = true;
                falseNickname = splited[1];
                std::cout << "RPL_ERROR_NICKNAME_IN_USE = " << RPL_ERROR_NICKNAME_IN_USE(client.getNickname(), falseNickname) << std::endl;
                return (sendCmd(RPL_ERROR_NICKNAME_IN_USE(client.getNickname(), falseNickname), client));
            }
        }
    }
    if (nickname_used == true)
    {
        nickname_used = false;
        std::cout << "RPL_CMD_NICK = " << RPL_CMD_NICK(falseNickname, client.getUsername(), client.getAddrIp(), splited[1]) << std::endl;
        sendBrodcastServer(RPL_CMD_NICK(client.getNickname(), client.getUsername(), client.getAddrIp(), splited[1]), server);
    }
    else
    {
        std::cout << "RPL_CMD_NICK = " << RPL_CMD_NICK(splited[1], client.getUsername(), client.getAddrIp(), splited[1]) << std::endl;
        sendBrodcastServer(RPL_CMD_NICK(client.getNickname(), client.getUsername(), client.getAddrIp(), splited[1]), server);
    }
    mapClients.find(client.getFd())->second.setNickname(splited[1]);
    client.setNickname(splited[1]);
    return ;
}

bool 	NickInit(std::string cmd, Clients& client, Server& server)
{
    std::cout << "NickInit" << std::endl;
    std::cout << "cmd : " << cmd << std::endl;
    static bool nickname_used = false;
    std::cout << "nickname_used = " << nickname_used << std::endl;
    static std::string falseNickname = "";
    std::cout << "falseNickname = " << falseNickname << std::endl;
    std::map<int, Clients>& mapClients = server.getClients();
    // std::map<int, Clients>::iterator itClients = mapClients.begin();
    std::vector<std::string> splited = split(cmd, ' ');
    if (splited.size() != 1)
    {
        std::cout << "ERR_NEEDMOREPARAMS = " << ERR_NEEDMOREPARAMS(client.getNickname(), "NICK") << std::endl;
        return (sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), "NICK"), client), false);
    }
    if (parseNick(splited[0], client) == false)
        return (false);
    std::cout << "splited[0] = " << splited[0] << std::endl;
    for (std::map<int, Clients>::iterator it = server.getClients().begin(); it != server.getClients().end(); it++)
    {
        std::cout << "it->second.getNickname() = " << it->second.getNickname() << std::endl;
        if (it->second.getNickname() == splited[0])
        {
            nickname_used = true;
            falseNickname = splited[0];
            std::cout << "RPL_ERROR_NICKNAME_IN_USE = " << RPL_ERROR_NICKNAME_IN_USE(client.getNickname(), falseNickname) << std::endl;
            return (sendCmd(RPL_ERROR_NICKNAME_IN_USE(client.getNickname(), falseNickname), client), false);
        }
    }
    if (nickname_used == true)
    {
        nickname_used = false;
        std::cout << "RPL_CMD_NICK = " << RPL_CMD_NICK(falseNickname, client.getUsername(), client.getAddrIp(), splited[0]) << std::endl;
        sendCmd(RPL_CMD_NICK(falseNickname, client.getUsername(), client.getAddrIp(), splited[0]), client);
    }
    else
    {
        std::cout << "RPL_CMD_NICK = " << RPL_CMD_NICK(splited[0], client.getUsername(), client.getAddrIp(), splited[0]) << std::endl;
        sendCmd(RPL_CMD_NICK(splited[0], client.getUsername(), client.getAddrIp(), splited[0]), client);
    }
    mapClients.find(client.getFd())->second.setNickname(splited[0]);
    client.setNickname(splited[0]);
    return (true);
}