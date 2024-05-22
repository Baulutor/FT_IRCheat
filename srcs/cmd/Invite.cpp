
#include <iostream>
#include "Cmd.hpp"

void    Invite(std::string cmd, Clients& client, Server& server)
{
	std::string nickname = &cmd[7];
	size_t spacefind;

	spacefind = nickname.find_first_of(" ");
	nickname = nickname.substr(0, spacefind);
	std::string channelName = &cmd[7 + nickname.size() + 1];
	channelName = channelName.substr(0, channelName.size() - 2);
	if (channelName[0] != '#')
		throw SendErrorExeption();

//	std::cout << "TEST cmd: " << cmd << " et nick: " << nickname << " spacefind = " << spacefind << " channelName:" << channelName << "-" << std::endl;
	std::cout << "ChannelSize: " << server.getChannels().size() << std::endl;
	std::cout << "ClientFd: " << client.getFd() << std::endl;

	std::map<std::string, Channels>::iterator it;
	std::map<std::string, Channels> serv = server.getChannels();

	// SI LE CLIENT QUI DEMANDE N'EST PAS DANS LE CHANNEL ERROR ICI (ou des les debut en soit lol)
	int i = 0;
	for (it = serv.begin(); it != serv.end(); it++)
	{
		i++;
		if (channelName == it->first)
		{
			//CLIENT qui demande l'invite, a t'il les droit ??? erreur ici sinon !
			std::map<int, Clients>::iterator ite;
			std::map<int, Clients> allClient = server.getClients();
			for (ite = allClient.begin(); ite != allClient.end(); ite++)
			{
				if (nickname == ite->second.getNickname())
				{
					std::map<std::string, Clients>::iterator iter;
					std::map<std::string, Clients> clientChannel = it->second.getClientMap();
					for (iter = clientChannel.begin(); iter != clientChannel.end(); iter++)
					{
						if (iter->second.getNickname() == nickname)
						{
							sendCmd(ERR_USERONCHANNEL(client.getNickname(), nickname, channelName), client); // ca coupe le serveur je pense pas qu'il faur que ca le fasse lol ;0
							throw SendErrorExeption();
						}
					}

					std::vector<Clients>::iterator itera;
					std::vector<Clients> &verif = it->second.getClientInvited();

					for (itera = verif.begin(); itera != verif.end(); itera++)
					{
						std::cout << "dans la boucle FOR" << std::endl;
						if (itera->getNickname() == nickname)
						{
							std::cout << "Error: a changer des barres" << std::endl;
							//ERROR: deja invite chelou lol
						}
					}
					std::cout << it->first << " AVANT D'ETRE INVITER LOL: " << it->second.getClientInvited().size() << std::endl;

					it->second.ClientInvite(ite->second);
					 // etre dans le vecteur = avoir ete inviter
					std::cout << it->first << " J'AI ETE INVITER LEt's GOOOO: " << it->second.getClientInvited().size() << std::endl;
					return ;
				}
				//ERROR NICKNAME NO FIND
			}
//		std::cout << "TEST first: " << it->first << ", second get name: " << it->second.getName() << std::endl;
		}
		// ERROR pas de channel TROUVE

	}
}
