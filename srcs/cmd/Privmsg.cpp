/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure < bfaure@student.42lyon.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 11:12:48 by bfaure            #+#    #+#             */
/*   Updated: 2024/06/02 18:52:57 by bfaure           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"

void Privmsg(std::string cmd, Clients& client, Server& server)
{
    std::cout << "PRIVMSG" << std::endl;
    std::vector<std::string> cmd_split = split(cmd, ' ');
    if (cmd_split.size() < 3)
        return (sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), "PRIVMSG"), client));
    if (cmd_split[2][0] != ':')
        return (sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), "PRIVMSG"), client));
    std::map<std::string, Channels>& channelsServer = server.getChannels();
    std::map<std::string, Channels>::iterator it = channelsServer.find(cmd_split[1]);
    std::map<int, Clients>& clientsServer = server.getClients();
    if (it == channelsServer.end())
    {
        if (server.getFdClientByName(cmd_split[1]) != -1)
            return (sendCmd(ERR_NOSUCHNICK(client.getNickname(), cmd_split[1]), client));
        return (sendCmd(ERR_NOSUCHCHANNEL(client.getNickname(), cmd_split[1]), client));
    }
    if (it != channelsServer.end() && it->second.getClientMap().find(client.getFd()) == it->second.getClientMap().end())
        return (sendCmd(ERR_CANNOTSENDTOCHAN(client.getNickname(), cmd_split[1]), client));
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
    if (it != channelsServer.end())
    {
        std::cout << "RPL_CMD_PRIVMSG" << RPL_CMD_PRIVMSG(client.getNickname(), client.getUsername(), client.getAddrIp(), cmd_split[1], cmd_split[2]) << std::endl;
        sendBroadcastMSG(RPL_CMD_PRIVMSG(client.getNickname(), client.getUsername(), client.getAddrIp(), cmd_split[1], cmd_split[2]), server.getChannels().find(cmd_split[1])->second, client);
    }
    else if (server.getFdClientByName(cmd_split[1]) != -1)
    {
        std::cout << "RPL_CMD_PRIVMSG" << RPL_CMD_PRIVMSG(client.getNickname(), client.getUsername(), client.getAddrIp(), cmd_split[1], cmd_split[2]) << std::endl;
        sendCmd(RPL_CMD_PRIVMSG(client.getNickname(), client.getUsername(), client.getAddrIp(), cmd_split[1], cmd_split[2]), findClientByName(cmd_split[1], clientsServer)->second);
    }
}
