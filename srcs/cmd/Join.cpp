/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:13:36 by bfaure            #+#    #+#             */
/*   Updated: 2024/06/01 19:02:39 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"
#include <algorithm>
#include <utility>

std::vector<std::string> make_keys(std::vector<std::string> splited)
{
    std::vector<std::string> keys;
    if (splited.size() > 2)
    {
        std::string key = splited.size() > 2 ? splited[2] : "";
        for (size_t i = 0; i < key.size(); i++)
        {
            if (key[i] == ',' && i + 1 < key.length() && key[i + 1] == ',')
            {
                key.insert(i + 1, " ");
                ++i;
            }
        }
        keys = split(key, ',');
        for (size_t i = 0; i < keys.size(); ++i)
        {
            if (keys[i] == " ")
                keys[i] = "";
        }
    }
    return (keys);
}

bool check_key(std::string key, Channels& channel, Clients& client)
{
    if (key != channel.getPassword() && channel.getMode(-42).find('k') != std::string::npos)
    {
        sendCmd(RPL_JOIN_ERROR_KEY(client.getNickname(), channel.getName()), client);
        return (false);
    }
    return (true);
}

bool check_invited(Channels& channel, Clients& client)
{
    std::map<int, Clients>::iterator it = channel.getClientInvited().find(client.getFd());
    // std::cout << "it = " << it->first << std::endl;
    std::cout << "channel.getMode(-42) = " << channel.getMode(-42) << std::endl;
    if (it == channel.getClientInvited().end() && channel.getMode(-42).find('i') != std::string::npos)
    {
        std::cout << "ERR_INVITEONLYCHAN = " << ERR_INVITEONLYCHAN(client.getNickname(), channel.getName()) << std::endl;
        sendCmd(ERR_INVITEONLYCHAN(client.getNickname(), channel.getName()), client);
        return (false);
    }
    return (true);
}

bool check_nb_clients(Channels& channel, Clients& client)
{
    if (channel.getNbClients() >= channel.getLimit() && channel.getMode(-42).find('l') != std::string::npos)
    {
        sendCmd(ERR_CHANNELFULL(client.getNickname(), channel.getName()), client);
        return (false);
    }
    return (true);
}

Channels& findChannel(std::string channel, std::map<std::string, Channels>& channelsServer, Clients& client)
{
    std::cout << "channelsServer.size() = " << channelsServer.size() << std::endl;
    for (std::map<std::string, Channels>::iterator it = channelsServer.begin(); it != channelsServer.end(); it++)
    {
        std::cout << "it->first = " << it->first << std::endl;
        if (it->first != channel)
        {
            Channels channelToJoin(channel, client);
            channelsServer.insert(std::make_pair(channel, channelToJoin));
            std::cout << "channelToJoin = " << channelsServer.find(channel)->second.getName() << std::endl;
            std::cout << "ChannelToJoin.getPassword = " << channelsServer.find(channel)->second.getPassword() << std::endl;
            return (channelsServer.find(channel)->second);
        }
        else
        {
            return (it->second);
        }
    }
    Channels channelToJoin(channel, client);
    channelsServer.insert(std::make_pair(channel, channelToJoin));
    return (channelsServer.find(channel)->second);
}

void Join(std::string cmd, Clients& client, Server& server)
{
    std::cout << "JOIN" << std::endl;
    std::vector<std::string> splited = split(cmd, ' ');
    std::vector<std::string> channels = split(splited[1], ',');
    std::vector<std::string> keys = make_keys(splited);
    std::string remove = "\r\n";
    std::map<std::string, Channels>& channelsServer = server.getChannels();
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
        std::cout << "channel = " << channel << std::endl;
        Channels& channelToJoin = findChannel(channel, channelsServer, client);
        if (check_key(key, channelToJoin, client) == false)
            continue;
        if (check_invited(channelToJoin, client) == false)
            continue;
        if (check_nb_clients(channelToJoin, client) == false)
            continue;
        std::pair<std::map<std::string, Channels>::iterator, bool> insertServer = channelsServer.insert(std::make_pair(channel, channelToJoin));
        if (insertServer.second)
            std::cout << "channel serveur ADD : " << insertServer.first->second.getName() << std::endl;
        std::pair<std::map<std::string, Channels>::iterator, bool> insertClient = server.getClients().find(client.getFd())->second.getChannelsClient().insert(std::make_pair(channel, channelToJoin));
        if (insertClient.second)
        {
            // insertClient.first->second.setOperator(client);
            // insertClient.first->second.setMode(client.getNickname(), " ");
            insertServer.first->second.setMode(client.getFd(), "");
            insertServer.first->second.incrementNbClients();
            std::cout << "Channel client ADD : " << insertClient.first->second.getName() << std::endl;
        }
        std::string Topic = insertClient.first->second.getTopic();
        std::string user;
        std::map<int, Clients>& serverMap = insertServer.first->second.getClientMap();
        serverMap.insert(std::make_pair(client.getFd(), client));
        std::map<int, Clients>& clientMap = insertClient.first->second.getClientMap();
        clientMap.insert(std::make_pair(client.getFd(), client));
        try
        {
            if (key != "")
            {
                std::cout << "RPL_CMD_JOIN = " << RPL_CMD_JOIN_KEY(client.getNickname(), client.getUsername(), client.getAddrIp(), channel, key) << std::endl;
                sendCmd(RPL_CMD_JOIN_KEY(client.getNickname(), client.getUsername(), client.getAddrIp(), channel, key), client);
            }
            else
            {
                std::cout << "RPL_CMD_JOIN = " << RPL_CMD_JOIN(client.getNickname(), client.getUsername(), client.getAddrIp(), channel) << std::endl;
                sendCmd(RPL_CMD_JOIN(client.getNickname(), client.getUsername(), client.getAddrIp(), channel), client);
            }
            
            //std::cout << "RPL_CMD_TOPIC = " << RPL_CMD_TOPIC(client.getNickname(), channel, Topic) << std::endl;
            sendCmd(RPL_CMD_TOPIC(client.getNickname(), channel, Topic), client);
            
            //std::cout << "NameLstUpadte channel = " << insertServer.first->second.getClientMap().find(client.getNickname())->second.getNickname() << std::endl;
            NameLstUpdate(client, insertServer.first->second);

            sendBroadcastChannel(RPL_JOIN_NOTICE(client.getNickname(), channel), insertServer.first->second);
        }
        catch (std::exception& e)
        {
            std::cout << "Error : " << e.what() << std::endl;
        }
    }
}
