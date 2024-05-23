
#include <iostream>
#include "Cmd.hpp"


// il manque dans le cas de invite dans un channel uniquement sur invitation et donc avec, l'erreur dans le cas le client n'as pas les droit d'ajouter !

void    Invite(std::string cmd, Clients& client, Server& server)
{
	std::string nickname = &cmd[7];
	size_t spacefind;

	spacefind = nickname.find_first_of(" ");
	nickname = nickname.substr(0, spacefind);
	std::string channelName = &cmd[7 + nickname.size() + 1];
	channelName = channelName.substr(0, channelName.size() - 2);

	if (channelName == client.getUsername())
	{
		sendCmd(ERR_NEEDMOREPARAMS(nickname, "INVITE"), client);
		return ;
	}

	std::map<std::string, Channels>::iterator it;
	std::map<std::string, Channels> &serv = server.getChannels();
	it = serv.find(channelName);
	if (it == serv.end())
	{
		sendCmd(ERR_NOSUCHCHANNEL(client.getNickname(), channelName), client);
		return;
	}
	if (it->second.getClientMap().end() == it->second.getClientMap().find(client.getNickname()))
	{
		sendCmd(ERR_NOTONCHANNEL(channelName), client);
		return ;
	}
	std::map<int, Clients>::iterator ite;
	std::map<int, Clients> allClient = server.getClients();
	for (ite = allClient.begin(); ite != allClient.end(); ite++)
	{
		if (nickname == ite->second.getNickname())
		{
			std::map<std::string, Clients> clientChannel = it->second.getClientMap();
			if (clientChannel.find(nickname) != clientChannel.end())
			{
				sendCmd(ERR_USERONCHANNEL(client.getNickname(), nickname, channelName), client);
				return ;
			}
			std::map<std::string, Clients> &verif = it->second.getClientInvited();
			// je crois qu'il n'y a aucune erreur lorsque l'on invite plusieurs fois le meme frero, dois-je afficher:
			// sendCmd(RPL_INVITING(nickname, channelName), client) dans ce if? (sans ajouter a la map du coup) our rien faire ?
			if (verif.find(nickname) != verif.end())
				return ;
			server.getChannels().find(it->first)->second.getClientInvited().insert(std::make_pair(ite->second.getNickname(), ite->second));
			sendCmd(RPL_INVITING(nickname, channelName), client);
			return ;
		}
	}
	sendCmd(ERR_NOSUCHNICK(nickname), client);
}
