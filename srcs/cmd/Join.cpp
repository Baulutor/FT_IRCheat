/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:13:36 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/13 17:23:37 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"
#include <algorithm>
#include <utility>

void Join(std::string cmd, Clients& client, std::map<std::string, Channels>& channelsServer)
{
    std::cout << "JOIN" << std::endl;
    std::vector<std::string> splited = split(cmd, ' ');
    std::vector<std::string> channels = split(splited[1], ',');
    std::string reponse;
    std::vector<std::string> keys;
    if (splited.size() > 2)
        keys = split(splited[2], ',');
    for (size_t i = 0; i < channels.size(); i++)
    {
        std::string key = (i < keys.size()) ? keys[i] : "";
        if (channels[i][0] == '#')
            channels[i] = channels[i].substr(1);
        std::map<std::string, Channels>::iterator it = channelsServer.find(channels[i]);
        if (it == channelsServer.end())
        {
            channelsServer.insert(std::make_pair(channels[i], Channels(channels[i], client)));
            std::cout << "channel serveur ADD : " << channelsServer[channels[i]].getName() << std::endl;
            std::map<std::string, Channels>& clientChannels = client.getChannels();
            std::map<std::string, Channels>::iterator it = clientChannels.find(channels[i]);
            if (it == clientChannels.end())
            {
                clientChannels.insert(std::make_pair(channels[i], Channels(channels[i], client)));
                std::cout << "Channel client ADD : " << clientChannels[channels[i]].getName() << std::endl;
            }
        }
        reponse = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getAddrIp() + " JOIN :" + channels[i] + " " + key + "\r\n";
        std::cout << "reponse : " << reponse << std::endl;
        if (send(client.getFd(), reponse.c_str(), reponse.size(), 0) == -1)
            throw (SendErrorExeption());
    }
}
