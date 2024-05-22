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
#include "IRC.h"
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

void	parsArg(char **argv) // LOL: j'ai cru qu'il fallait toutes ces regles pour le mot de passe alors que c'est pour le nickname hahahahaha ;)
{
	std::string pars = argv[2];
	// UTILE JE PENSE LA TEAM PAS VRAI ??
	if (argv[2][0] == '\0')
		throw std::invalid_argument("Error: Pass can't be NULL");
	if (pars.size() > 9)
		throw std::invalid_argument("Error: Pass cannot contain more than 9 character");

	// among us...
	if (pars.find_first_not_of(", *?!@"))
		throw std::invalid_argument("Error: Pass cannot contain these character (\',\' \' \' \'*\' \'?\' \'!\' \'@\')");
	if (pars[0] == '&' || pars[0] == '#')
		throw std::invalid_argument("Error: Pass cannot start with \'&\' or \'#\'");

	// EN VRAI:: ca c'est pas faux je crois la team
	if (pars.find_first_not_of("0123456789") != pars.npos)
		throw std::invalid_argument("Error: Please only put number in your Port");
	long buf = atoi(pars.c_str());
	if (buf > INT_MAX)
		throw std::invalid_argument("Error: Please only put number in your Port");

}
