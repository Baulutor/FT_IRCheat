/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PRVMSG.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 11:12:48 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/21 11:34:37 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"

void Prvmsg(std::string cmd, Clients& client, Server& server)
{
    std::vector<std::string> cmd_split = split(cmd, ' ');
    std::map<std::string, Channels>& channelsServer = server.getChannels();
    std::map<std::string, Channels>::iterator it = channelsServer.find(cmd_split[1]);
    if (it != channelsServer.end())
    {
        for (std::map<std::string, Clients>::iterator itClient = it->second.getClientMap().begin(); itClient != it->second.getClientMap().end(); itClient++)
        {
            sendCmd(RPL_CMD_PRIVMSG(client.getNickname(), client.getUsername(), client.getAddrIp(), "PRVMSG", cmd_split[2]), itClient->second);
        }
    }
}
