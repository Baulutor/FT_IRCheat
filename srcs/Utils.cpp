/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 11:59:26 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/31 18:31:22 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "FT_IRCheat.hpp"
#include "RPL.hpp"
// #include "Clients.hpp"

std::vector<std::string> splitEOF(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    std::istringstream iss(s);
    std::string item;
    while (std::getline(iss, item, delim))
    {
        if (!item.empty())
        {
            std::cout << "item = " << item << std::endl;
            elems.push_back(item);
        }
    }
    return (elems);
}

// std::vector<std::string> split(const std::string &s, const std::string &delim)
// {
//     std::vector<std::string> elems;
//     std::string::size_type pos = 0;
//     std::string::size_type prev = 0;

//     while ((pos = s.find_first_of(delim, prev)) != std::string::npos)
//     {
//         if (pos > prev)
//             elems.push_back(s.substr(prev, pos - prev));
//         prev = pos + 1;
//     }

//     if (prev < s.length())
//         elems.push_back(s.substr(prev));

//     return elems;
// }

std::vector<std::string> splitInit(const std::string &line, char delim)
{
    std::vector<std::string> elems;
    std::istringstream iss(line);
    std::string item;
    while (std::getline(iss, item, delim))
    {
        size_t endpos = item.find("\r\n");
        if (endpos != std::string::npos)
        {
            item.erase(endpos, 2);
            std::string keywords[] = {"WHO", "MODE"};
            bool spaceInserted = false;
            for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); ++i)
            {
                size_t pos = item.find(keywords[i], endpos);
                if (pos == endpos)
                {
                    item.insert(pos, " ");
                    spaceInserted = true;
                    break ;
                }
            }
            if (spaceInserted)
            {
                std::istringstream iss2(item);
                std::string subitem;
                while (std::getline(iss2, subitem, ' '))
                {
                    if (!subitem.empty())
                        elems.push_back(subitem);
                }
                continue;
            }
        }
        if (!item.empty())
            elems.push_back(item);
    }
    return (elems);
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    std::istringstream iss(s);
    std::string item;
    std::string remove = "\r\n";
    while (std::getline(iss, item, delim))
    {
        size_t endpos = item.find(remove);
        if (endpos != std::string::npos && item.find("\n") == endpos + 1 && item.find("\r") == endpos)
            item.erase(endpos, remove.size());

        if (!item.empty())
            elems.push_back(item);
    }
    return (elems);
}

std::string intToString(int value) {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void signalHandler(int signum)
{
	(void)signum; // a differencier ctrl C et ctrl D
	throw std::invalid_argument("CHELOU j'ai bien envoyer lol");
}

void sendCmd(const std::string& cmd, Clients &client)
{
    if (send(client.getFd(), cmd.c_str(), cmd.size(), MSG_NOSIGNAL | MSG_DONTWAIT) < 0)
        std::cerr << "Error: send() failed" << std::endl;
        // throw std::exception();
}

void sendBrodcastChannel(const std::string& cmd, Channels& channel)
{
	for (std::map<int, Clients>::iterator it = channel.getClientMap().begin(); it != channel.getClientMap().end(); ++it)
	{
		if (send(it->first, cmd.c_str(), cmd.size(), MSG_NOSIGNAL | MSG_DONTWAIT) < 0)
            std::cerr << "Error: send() failed" << std::endl;
			// throw std::exception();
	}
}

void sendBrodcastMSG(const std::string& cmd, Channels& channel, Clients& client)
{
    for (std::map<int, Clients>::iterator it = channel.getClientMap().begin(); it != channel.getClientMap().end(); ++it)
    {
        if (it->first != client.getFd())
        {
            if (send(it->second.getFd(), cmd.c_str(), cmd.size(), MSG_NOSIGNAL | MSG_DONTWAIT) < 0)
                std::cerr << "Error: send() failed" << std::endl;
                // throw std::exception();
        }
    }
}

void sendBrodcastServer(const std::string& cmd, Server& server)
{
	for (std::map<int, Clients>::iterator it = server.getClients().begin(); it != server.getClients().end(); ++it)
	{
		if (send(it->first, cmd.c_str(), cmd.size(), MSG_NOSIGNAL | MSG_DONTWAIT) < 0)
			std::cerr << "Error: send() failed" << std::endl;
			// throw std::exception();
	}
}

void NameLstUpadte(Clients& client, Channels& channel)
{
	std::string user;
	for (std::map<int, Clients>::iterator it = channel.getClientMap().begin(); it != channel.getClientMap().end(); it++)
	{
		if (channel.getOperator(it->second.getNickname()).getNickname() == it->second.getNickname())
			user += "@" + it->second.getNickname();
		else
			user += it->second.getNickname();
		std::map<int, Clients>::iterator ite = channel.getClientMap().end();
		ite--;
		if (it != ite)
			user += " ";
	}
	std::cout << "RPL_CMD_NAME_LST_START = " << RPL_CMD_NAME_LST_START(client.getNickname(), channel.getName(), user) << std::endl;
    sendBrodcastChannel(RPL_CMD_NAME_LST_START(client.getNickname(), channel.getName(), user), channel);
	
    std::cout << "RPL_CMD_NAME_LST_END = " << RPL_CMD_NAME_LST_END(client.getNickname(), channel.getName()) << std::endl;
    sendBrodcastChannel(RPL_CMD_NAME_LST_END(client.getNickname(), channel.getName()), channel);
}

int findFdClientByName(std::string nickname, std::map<int, Clients>& clientsServer)
{
    for (std::map<int, Clients>::iterator it = clientsServer.begin(); it != clientsServer.end(); ++it)
    {
        if (it->second.getNickname() == nickname)
            return (it->first);
    }
    return (-1);
}

std::map<int, Clients>::iterator findClientByName(std::string nickname, std::map<int, Clients>& clientsServer)
{
    for (std::map<int, Clients>::iterator it = clientsServer.begin(); it != clientsServer.end(); ++it)
    {
        if (it->second.getNickname() == nickname)
            return (it);
    }
    return (clientsServer.end());
}

// bool isClientInChannel(std::string nickname, Channels& channel)
// {
// 	if (channel.getClientMap().find(nickname) != channel.getClientMap().end())
// 		return (true);
// 	return (false);
//     // for (std::map<std::string, Clients>::iterator it = channel.getClientMap().begin(); it != channel.getClientMap().end(); ++it)
//     // {
//     //     if (it->second.getNickname() == nickname)
//     //         return (true);
//     // }
//     // return (false);
// }

std::map<std::string, Channels>::iterator findChannelByName(std::string channelName, std::map<std::string, Channels>& channelsServer)
{
    for (std::map<std::string, Channels>::iterator it = channelsServer.begin(); it != channelsServer.end(); ++it)
    {
        if (it->second.getName() == channelName)
            return (it);
    }
    return (channelsServer.end());
}