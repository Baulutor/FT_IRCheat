/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:42:28 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/25 12:20:24 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"

bool isChannel(std::string target)
{
    if (target.find('#') != std::string::npos)
        return (true);
    return (false);
}

void Mode(std::string cmd, Clients& client, Server& server)
{
    std::cout << "Mode" << std::endl;
    std::vector<std::string> tokens = split(cmd, ' ');
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++)
        std::cout << "tokens: |" << *it << "|" << std::endl;
    if (tokens.size() < 2)
        return (sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), tokens[0]), client));
    if (tokens.size() == 2)
    {
        std::map<std::string, Channels>::iterator channelIt = server.getChannels().find(tokens[1]);
        if (channelIt == server.getChannels().end())
            return (sendCmd(ERR_NOSUCHCHANNEL(client.getNickname(), tokens[1]), client));
        return (sendCmd(RPL_CHANNELMODEIS(client.getNickname(), tokens[1], channelIt->second.getMode(tokens[1])), client));
    }
    if (tokens.size() == 3)
    {
        std::map<std::string, Channels>::iterator channelIt = server.getChannels().find(tokens[1]);
        if (channelIt == server.getChannels().end())
            return (sendCmd(ERR_NOSUCHCHANNEL(client.getNickname(), tokens[1]), client));
        std::map<std::string, Clients>::iterator clientIt = channelIt->second.getClientMap().find(tokens[2]);
        if (clientIt == channelIt->second.getClientMap().end())
            return (sendCmd(ERR_NOSUCHNICK(client.getNickname(), tokens[2]), client));
        return (sendCmd(RPL_UMODEIS(client.getNickname(), tokens[2], channelIt->second.getMode(tokens[2])), client));
    }
}

