/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:15:48 by nibernar          #+#    #+#             */
/*   Updated: 2024/06/01 00:37:01 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"

bool    parseUser(std::string& user, Clients& client)
{
    std::cout << "parseUser" << std::endl;
    if (user.size() == 0 || user.size() > 12)
    {
        std::cout << "RPL_ISUPPORT = " << RPL_ISUPPORT(client.getNickname(), "USERLEN=12") << std::endl;
        sendCmd(RPL_ISUPPORT(client.getNickname(), "USERLEN=12"), client);
        return false;
    }
    for (size_t i = 0; i < user.size(); i++)
    {
        if (user[i] == ',' || user[i] == ' ' || user[i] == '*' || user[i] == '?' || user[i] == '!' || user[i] == '@' || (user[i] >= '0' && user[i] <= '9'))
        {
            std::cout << "ERR_ERRONEUSNICKNAME = " << ERR_ERRONEUSNICKNAME(client.getNickname(), user) << std::endl;
            sendCmd(ERR_ERRONEUSNICKNAME(client.getNickname(), user), client);
            return false;
        }
    }
    if (user[0] == '#' || user[0] == '&' || user[0] == ':')
    {
        std::cout << "ERR_ERRONEUSNICKNAME = " << ERR_ERRONEUSNICKNAME(client.getNickname(), user) << std::endl;
        sendCmd(ERR_ERRONEUSNICKNAME(client.getNickname(), user), client);
        return false;
    }
    return true;
}

bool    User(std::string cmd, Clients& client, Server& server)
{
    std::cout << "USER" << std::endl;
    std::cout << "cmd: " << cmd << std::endl;
    std::map<int, Clients>& mapClients = server.getClients();
    std::map<int, Clients>::iterator itClients = mapClients.begin();
    std::vector<std::string> splited = split(cmd, ' ');
    if (splited.size() != 4)
        return (sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), "USER"), client), false);
    if (splited[1].size() != 1 && splited[1][0] != '0')
        return (sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), "USER"), client), false);
    if (splited[2].size() != 1 && splited[2][0] != '*')
        return (sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), "USER"), client), false);
    if (splited[3][0] != ':')
        return (sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), "USER"), client), false);
    if (client.getIsRegistered() == true)
        return (sendCmd(ERR_ALREADYREGISTERED(client.getNickname()), client), false);
    if (splited[3][0] == ':')
    {
        splited[3].erase(0, 1);
        std::string msg;
        for (size_t i = 3; i < splited.size(); i++)
        {
            if (i != splited.size() - 1)
                msg += splited[i] + " ";
            else
                msg += splited[i];
        }
        splited[3] = msg;
        splited.resize(4);
    }
    if (client.getUsername().size() != 0)
        if (itClients->second.getUsername() == splited[0])
            return (sendCmd(RPL_ERROR_NICKNAME_IN_USE(client.getUsername(), itClients->second.getUsername()), client), false);
    if (parseUser(splited[0], client) == true)
    {
        client.setUsername(splited[0]);
        std::cout << "Username = " << client.getUsername() << std::endl;
        //sendCmd(RPL_CMD_NICK(falseNickname, getUsername(), getAddrIp(), tokens[i + 1]), client);
    }
    return (true);
}