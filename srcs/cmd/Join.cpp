/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:13:36 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/15 18:26:08 by bfaure           ###   ########.fr       */
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
        std::cout << "PASS1" << std::endl;
        std::string key = (i < keys.size()) ? keys[i] : "";
        // if (channels[i][0] == '#')
        //     channels[i] = channels[i].substr(1);
        std::cout << "PASS2" << std::endl;
        std::map<std::string, Channels>::iterator serverIt = channelsServer.find(channels[i]);
        if (serverIt == channelsServer.end())
        {
            std::pair<std::map<std::string, Channels>::iterator, bool> insert = channelsServer.insert(std::make_pair(channels[i], Channels(channels[i], client)));
            serverIt = insert.first;
            serverIt->second.setOperator(client);
            std::cout << "channel serveur ADD : " << insert.first->second.getName() << std::endl;
        }
        // server.setChannels(channelsServer);
        std::cout << "PASS3" << std::endl;
        std::map<std::string, Channels>::iterator clientIt = clientChannels.find(channels[i]);
        std::map<std::string, Clients> clientMap;
        if (clientIt == clientChannels.end())
        {
            std::pair<std::map<std::string, Channels>::iterator, bool> insert = clientChannels.insert(std::make_pair(channels[i], Channels(channels[i], client)));
            clientIt = insert.first;
            clientMap = clientIt->second.getClientMap();
            // insert.first->second.setTopic("Welcome");
            clientIt->second.setOperator(client);
            server.getChannels().find(channels[i])->second.setOperator(client);
            client.getChannels().find(channels[i])->second.setOperator(client);
            clientMap.insert(std::make_pair(client.getNickname(), client));
            std::cout << "Channel client ADD : " << clientIt->second.getName() << std::endl;
        }
        channelsServer.find(channels[i])->second.setClients(clientMap);
        server.setChannels(channelsServer);
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
            if (serverIt->second.getOperator().getNickname() == it->second.getNickname())
                user += "@" + it->second.getNickname() + " ";
            else
                user += it->second.getNickname() + " ";
        }
        std::string reponseNameLstStart = ":server 375 " + client.getNickname() + " = " + channels[i] + " :" + user + "\r\n";
        std::cout << "reponseNameLstStart : " << reponseNameLstStart << std::endl;
        if (send(client.getFd(), reponseNameLstStart.c_str(), reponseNameLstStart.size(), 0) == -1)
            throw (SendErrorExeption());
        std::string reponseNameLstEnd = ":server 376 " + client.getNickname() + " " + channels[i] + " :End of /NAMES list" + "\r\n";
        std::cout << "reponseNameLstEnd : " << reponseNameLstEnd << std::endl;
        if (send(client.getFd(), reponseNameLstEnd.c_str(), reponseNameLstEnd.size(), 0) == -1)
            throw (SendErrorExeption());
    }
    std::cout << "client in chan test name : " << client.getChannels().find("test8")->second.getOperator().getNickname() << std::endl;
}
