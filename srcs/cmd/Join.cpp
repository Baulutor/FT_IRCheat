/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:13:36 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/16 14:06:43 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"
#include <algorithm>
#include <utility>

void Join(std::string cmd, Clients& client, Server& server)
{
    std::cout << "JOIN" << std::endl;
    std::vector<std::string> splited = split(cmd, ' ');
    std::vector<std::string> channels = split(splited[1], ',');
    std::string reponse;
    std::vector<std::string> keys;
    std::string remove = "\r\n";
    std::map<std::string, Channels>& channelsServer = server.getChannels();
    //std::map<std::string, Channels>& clientChannels = client.getChannels();
    if (splited.size() > 2)
        keys = split(splited[2], ',');
    size_t pos;
    for (size_t i = 0; i < channels.size(); i++)
    {
        if ((pos = channels[i].find(remove)) != std::string::npos)
            channels[i].erase(pos, remove.length());
        std::string key = (i < keys.size()) ? keys[i] : "";
        std::string channel = channels[i];
        std::pair<std::map<std::string, Channels>::iterator, bool> insertServer = channelsServer.insert(std::make_pair(channels[i], Channels(channels[i], client)));
        if (insertServer.second)
        {
            insertServer.first->second.setOperator(client);
            std::cout << "channel serveur ADD : " << insertServer.first->second.getName() << std::endl;
        }

        std::pair<std::map<std::string, Channels>::iterator, bool> insertClient = channelsServer.insert(std::make_pair(channel, Channels(channel, client)));
        if (insertClient.second)
        {
            insertClient.first->second.setOperator(client);
            std::cout << "Channel client ADD : " << insertClient.first->second.getName() << std::endl;
        }

        sendCmd(RPL_CMD_JOIN(client.getNickname(), client.getUsername(), client.getAddrIp(), channel, key), client);
        std::string Topic = insertClient.first->second.getTopic();

        sendCmd(RPL_CMD_TOPIC(client.getNickname(), channel, Topic), client);
        std::string user;
        std::map<std::string, Clients>& clientMap = insertClient.first->second.getClientMap();
        clientMap.insert(std::make_pair(client.getNickname(), client));
        std::cout << "clientMap.size() = " << clientMap.size() << std::endl;
        for (std::map<std::string, Clients>::iterator it = clientMap.begin(); it != clientMap.end(); it++)
        {
            std::cout << "it->second.getNickname() = " << it->second.getNickname() << std::endl;
            if (insertServer.first->second.getOperator().getNickname() == it->second.getNickname())
                user += "@" + it->second.getNickname() + " ";
            else
                user += it->second.getNickname() + " ";
        }

        std::cout << "RPL_CMD_NAME_LST_START = " << RPL_CMD_NAME_LST_START(client.getNickname(), channel, user) << std::endl;
        sendCmd(RPL_CMD_NAME_LST_START(client.getNickname(), channel, user), client);

        std::cout << "RPL_CMD_NAME_LST_END = " << RPL_CMD_NAME_LST_END(client.getNickname(), channel) << std::endl;
        sendCmd(RPL_CMD_NAME_LST_END(client.getNickname(), channel), client);
    }
}
