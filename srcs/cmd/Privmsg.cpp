/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 11:12:48 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/23 14:07:01 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"

void Privmsg(std::string cmd, Clients& client, Server& server)
{
    std::cout << "PRIVMSG" << std::endl;
    std::vector<std::string> cmd_split = split(cmd, ' ');
    if (cmd_split[2][0] == ':')
    {
        cmd_split[2].erase(0, 1);
        std::string msg;
        for (size_t i = 2; i < cmd_split.size(); i++)
        {
            if (i != cmd_split.size() - 1)
                msg += cmd_split[i] + " ";
            else
                msg += cmd_split[i];
        }
        cmd_split[2] = msg;
        cmd_split.resize(3);
    }
    std::map<std::string, Channels>& channelsServer = server.getChannels();
    std::map<int, Clients>& clientsServer = server.getClients();
    std::map<std::string, Channels>::iterator it = channelsServer.find(cmd_split[1]);
    if (it != channelsServer.end())
    {
        std::cout << "RPL_CMD_PRIVMSG" << RPL_CMD_PRIVMSG(client.getNickname(), client.getUsername(), client.getAddrIp(), cmd_split[1], cmd_split[2]) << std::endl;
        sendBrodcastMSG(RPL_CMD_PRIVMSG(client.getNickname(), client.getUsername(), client.getAddrIp(), cmd_split[1], cmd_split[2]), it->second, client);
    }
    else if (findFdClientByName(cmd_split[1], clientsServer) != -1)
    {
        std::cout << "RPL_CMD_PRIVMSG" << RPL_CMD_PRIVMSG(client.getNickname(), client.getUsername(), client.getAddrIp(), cmd_split[1], cmd_split[2]) << std::endl;
        sendCmd(RPL_CMD_PRIVMSG(client.getNickname(), client.getUsername(), client.getAddrIp(), cmd_split[1], cmd_split[2]), findClientByName(cmd_split[1], clientsServer));
    }
}
