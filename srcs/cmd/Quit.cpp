//
// Created by dbaule on 5/30/24.
//

#include "Cmd.hpp"

void 	Quit(std::string cmd, Clients& client, Server& server)
{

	std::cout << "["<< cmd << "]" <<  std::endl;
	std::map<std::string, Channels> channels = client.getChannels();
	std::map<std::string, Channels>::iterator chan = channels.begin();


	for (; chan != channels.end(); chan++)
	{
		std::map<std::string, Clients> clientMap = chan->second.getClientMap();
		std::map<std::string, Clients> clientInvitedMap = chan->second.getClientInvited();
		std::map<std::string, Clients>::iterator ok = clientMap.find(client.getNickname());
		std::vector<Clients> opeVec = chan->second.getOperatorVector();
		for (size_t i = 0; i < opeVec.size(); i++)
		{
			std::cout << opeVec[i].getNickname() << ": c'est les differents frero dans le bail operateur !!!!!" << std::endl;
		}
		std::cout << "chan->first: " << chan->first << " , valeur de opeVec.size(): " << opeVec.size() << std::endl;
		if (opeVec.size() == 1)
		{
			if (opeVec.begin()->getNickname() == client.getNickname())
			{
				if (clientMap.size() == 1)
				{
					std::cout << "cas de channel seul !" << std::endl;
					client.getChannels().find(chan->first)->second.getClientInvited().clear();
					client.getChannels().find(chan->first)->second.getClientMap().clear();
					client.getChannels().find(chan->first)->second.getOperatorVector().clear();
					client.getChannels().clear();
					client.getChannelsInvite().clear();
					return ;
				}
				else
				{
					for (std::map<std::string, Clients>::iterator it = clientMap.begin(); it != clientMap.end(); it++)
					{
						if (it->first != client.getNickname())
						{
//							client.getChannels().find(chan->first)->second.setOperator(it->second);
							break ;
						}
					}
					client.getChannels().find(chan->first)->second.removeOperator(client);
				}
			}
		}
		server.getChannels().find(chan->first)->second.getClientMap().erase(ok);
		std::map<std::string, Clients>::iterator invitedMapIt = server.getChannels().find(chan->first)->second.getClientInvited().find(client.getNickname());
		if (invitedMapIt != clientInvitedMap.end())
		{
			if (clientInvitedMap.size() == 1)
				server.getChannels().find(chan->first)->second.getClientInvited().clear();
			else
				server.getChannels().find(chan->first)->second.getClientInvited().erase(invitedMapIt);
		}
		int fdClient = client.getFd();
		std::vector<pollfd> pollFdServ = server.getLstPollFd();
		for (size_t i = 0; i < pollFdServ.size() - 1; i++)
		{
			if (pollFdServ[i].fd == fdClient)
			{
				close(server.getLstPollFd().at(i).fd);
				std::vector<pollfd>::iterator it = pollFdServ.begin() + i;
				server.getLstPollFd().erase(it);
				break ;
			}
		}
	}

	(void)client;
	(void)server;
}
