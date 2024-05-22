/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:59:26 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/22 13:23:21 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FT_IRCheat.hpp"
#include "RPL.hpp"
// #include "Clients.hpp"

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim))
    {
        if (!item.empty())
            elems.push_back(item);
    }
    return (elems);
}

void sendCmd(const std::string& cmd, Clients &client)
{
    if (send(client.getFd(), cmd.c_str(), cmd.size(), 0) < 0)
        throw std::exception();
}

void sendBrodcastChannel(const std::string& cmd, Channels& channel)
{
	for (std::map<std::string, Clients>::iterator it = channel.getClientMap().begin(); it != channel.getClientMap().end(); ++it)
	{
		if (send(it->second.getFd(), cmd.c_str(), cmd.size(), 0) < 0)
			throw std::exception();
	}
}

void sendBrodcastMSG(const std::string& cmd, Channels& channel, Clients& client)
{
    for (std::map<std::string, Clients>::iterator it = channel.getClientMap().begin(); it != channel.getClientMap().end(); ++it)
    {
        if (it->second.getFd() != client.getFd())
        {
            if (send(it->second.getFd(), cmd.c_str(), cmd.size(), 0) < 0)
                throw std::exception();
        }
    }
}

void sendBrodcastServer(const std::string& cmd, Server& server)
{
	for (std::map<int, Clients>::iterator it = server.getClients().begin(); it != server.getClients().end(); ++it)
	{
		if (send(it->first, cmd.c_str(), cmd.size(), 0) < 0)
			throw std::exception();
	}
}
