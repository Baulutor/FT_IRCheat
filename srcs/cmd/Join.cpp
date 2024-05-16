/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:13:36 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/16 11:30:08 by bfaure           ###   ########.fr       */
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
    std::map<std::string, Channels>& clientChannels = client.getChannels();
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
        std::pair<std::map<std::string, Channels>::iterator, bool> insertClient = clientChannels.insert(std::make_pair(channel, Channels(channel, client)));
        if (insertClient.second)
        {
            insertClient.first->second.setOperator(client);
            std::cout << "Channel client ADD : " << insertClient.first->second.getName() << std::endl;
        }



        //=============
        // std::pair<std::map<std::string, Channels>::iterator, bool> insertClient = channelsServer.insert(std::make_pair(channel, Channels(channel, client)));
        // if (insertClient.second)
        // {
        //     insertClient.first->second.setOperator(client);
        //     std::cout << "Channel client ADD : " << insertClient.first->second.getName() << std::endl;
        // }
        //=============



        // reponse = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getAddrIp() + " JOIN :" + channel + " " + key + "\r\n";
        // std::cout << "reponse : " << reponse << std::endl;
        // if (send(client.getFd(), reponse.c_str(), reponse.size(), 0) == -1)
        //     throw (SendErrorExeption());
        sendCmd(RPL_CMD_JOIN(client.getNickname(), client.getUsername(), client.getAddrIp(), channel, key), client);
        std::string Topic = insertClient.first->second.getTopic();
        // std::string reponseWho = ":server 332 " + client.getNickname() + " " + channel + " :" + Topic + "\r\n";
        // std::cout << "reponseWho : " << reponseWho << std::endl;
        // if (send(client.getFd(), reponseWho.c_str(), reponseWho.size(), 0) == -1)
        //     throw (SendErrorExeption());
        sendCmd(RPL_CMD_TOPIC(client.getNickname(), channel, Topic), client);
        std::string user;
        std::map<std::string, Clients>& clientMap = insertClient.first->second.getClientMap();
        for (std::map<std::string, Clients>::iterator it = clientMap.begin(); it != clientMap.end(); it++)
        {
            std::cout << "icccci = " << it->first << std::endl;
            if (insertServer.first->second.getOperator().getNickname() == it->second.getNickname())
                user += "@" + it->second.getNickname() + " ";
            else
                user += it->second.getNickname() + " ";
        }
        // std::string reponseNameLstStart = ":server 375 " + client.getNickname() + " = " + channel + " :" + user + "\r\n";
        // std::cout << "reponseNameLstStart : " << reponseNameLstStart << std::endl;
        // if (send(client.getFd(), reponseNameLstStart.c_str(), reponseNameLstStart.size(), 0) == -1)
        //     throw (SendErrorExeption());
        sendCmd(RPL_CMD_NAME_LST_START(client.getNickname(), channel), client);
        // std::string reponseNameLstEnd = ":server 376 " + client.getNickname() + " " + channel + " :End of /NAMES list" + "\r\n";
        // std::cout << "reponseNameLstEnd : " << reponseNameLstEnd << std::endl;
        // if (send(client.getFd(), reponseNameLstEnd.c_str(), reponseNameLstEnd.size(), 0) == -1)
        //     throw (SendErrorExeption());
        sendCmd(RPL_CMD_NAME_LST_END(client.getNickname()), client);
    }
    //std::cout << "client in chan test name : " << client.getChannels().find("test8")->second.getOperator().getNickname() << std::endl;
}
