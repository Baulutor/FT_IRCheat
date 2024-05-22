/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:13:36 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/22 16:19:42 by bfaure           ###   ########.fr       */
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
    std::vector<std::string> keys;
    std::string remove = "\r\n";
    std::map<std::string, Channels>& channelsServer = server.getChannels();
    if (splited.size() > 2)
        keys = split(splited[2], ',');
    size_t pos;
    for (size_t i = 0; i < channels.size(); i++)
    {
        if ((pos = channels[i].find(remove)) != std::string::npos)
            channels[i].erase(pos, remove.length());
        if (channels[i][0] != '#')
        {
            sendCmd(RPL_JOIN_ERROR_NAME(client.getNickname(), channels[i]), client);
            continue;
        }
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
        std::string Topic = insertClient.first->second.getTopic();
        std::string user;
        std::map<std::string, Clients>& clientMap = insertClient.first->second.getClientMap();
        clientMap.insert(std::make_pair(client.getNickname(), client));
        try
        {
            std::cout << "RPL_CMD_JOIN = " << RPL_CMD_JOIN(client.getNickname(), client.getUsername(), client.getAddrIp(), channel, key) << std::endl;
            sendCmd(RPL_CMD_JOIN(client.getNickname(), client.getUsername(), client.getAddrIp(), channel, key), client);
            
            std::cout << "RPL_CMD_TOPIC = " << RPL_CMD_TOPIC(client.getNickname(), channel, Topic) << std::endl;
            sendCmd(RPL_CMD_TOPIC(client.getNickname(), channel, Topic), client);
            
            NameLstUpadte(client, insertClient.first->second);

            sendBrodcastChannel(RPL_JOIN_NOTICE(client.getNickname(), channel), insertClient.first->second);
        }
        catch (std::exception& e)
        {
            std::cout << "Error : " << e.what() << std::endl;
        }
    }
}
