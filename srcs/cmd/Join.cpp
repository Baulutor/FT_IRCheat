/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:13:36 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/14 17:08:41 by bfaure           ###   ########.fr       */
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
        {
            std::cout << "pos = " << pos << std::endl;
            channels[i].erase(pos, remove.length());
            std::cout << "channels[i] = |" << channels[i] << "|" << std::endl;
        }
        std::string key = (i < keys.size()) ? keys[i] : "";
        // if (channels[i][0] == '#')
        //     channels[i] = channels[i].substr(1);
        std::map<std::string, Channels>::iterator serverIt = channelsServer.find(channels[i]);
        if (serverIt == channelsServer.end())
        {
            channelsServer.insert(std::make_pair(channels[i], Channels(channels[i], client)));
            std::cout << "channel serveur ADD : " << serverIt->second.getName() << std::endl;
        }
        std::map<std::string, Channels>::iterator clientIt = clientChannels.find(channels[i]);
        std::map<std::string, Clients> clientMap;
        if (clientIt == clientChannels.end())
        {
            clientChannels.insert(std::make_pair(channels[i], Channels(channels[i], client)));
            clientMap = clientChannels[channels[i]].getClientMap();
            clientIt->second.setTopic("Welcome");
            clientMap.insert(std::make_pair(client.getNickname(), client));
            std::cout << "Channel client ADD : " << clientIt->second.getName() << std::endl;
        }
        reponse = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getAddrIp() + " JOIN :" + channels[i] + " " + key + "\r\n";
        std::cout << "reponse : " << reponse << std::endl;
        if (send(client.getFd(), reponse.c_str(), reponse.size(), 0) == -1)
            throw (SendErrorExeption());
        std::string reponseWho = ":server 332 " + client.getNickname() + " " + channels[i] + " :" + clientIt->second.getTopic() + "\r\n";
        std::cout << "reponseWho : " << reponseWho << std::endl;
        if (send(client.getFd(), reponseWho.c_str(), reponseWho.size(), 0) == -1)
            throw (SendErrorExeption());
        std::string user;
        for (std::map<std::string, Clients>::iterator it = clientMap.begin(); it != clientMap.end(); it++)
        {
            user += it->second.getNickname() + " ";
        }
        std::string reponseNameLstStart = ":server 353 " + client.getNickname() + " = " + channels[i] + " : " + user + "\r\n";
        if (send(client.getFd(), reponseNameLstStart.c_str(), reponseNameLstStart.size(), 0) == -1)
            throw (SendErrorExeption());
        std::string reponseNameLstEnd = ":server 366 " + client.getNickname() + " " + channels[i] + " :End of /NAMES list\r\n";
        if (send(client.getFd(), reponseNameLstEnd.c_str(), reponseNameLstEnd.size(), 0) == -1)
            throw (SendErrorExeption());
    }
}
