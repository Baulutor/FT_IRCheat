//
// Created by dbaule on 5/30/24.
//

#include "Cmd.hpp"

#include "Cmd.hpp"

void Quit(std::string cmd, Clients& client, Server& server)
{

	std::cout << "["<< cmd << "]" <<  std::endl;
	std::map<std::string, Channels> channels = client.getChannelsClient();
	std::map<std::string, Channels>::iterator chan = channels.begin();


	for (; chan != channels.end(); chan++)
	{
		std::map<int, Clients> clientMap = chan->second.getClientMap();
		std::map<int, Clients> clientInvitedMap = chan->second.getClientInvited();
		std::map<int, Clients>::iterator ok = clientMap.find(client.getFd());
		std::vector<Clients> opeVec = chan->second.getOperatorVector();
//		for (size_t i = 0; i < opeVec.size(); i++)
//		{
//			std::cout << opeVec[i].getFd() << ": c'est les differents frero dans le bail operateur !!!!!" << std::endl;
//		}
		std::cout << "chan->first: " << chan->first << " , valeur de opeVec.size(): " << opeVec.size() << std::endl;
		if (opeVec.size() == 1)
		{
			if (opeVec.begin()->getFd() == client.getFd())
			{
				if (clientMap.size() == 1)
				{
					std::cout << "cas de channel seul !" << std::endl;
					client.getChannelsClient().find(chan->first)->second.getClientInvited().clear();
					client.getChannelsClient().find(chan->first)->second.getClientMap().clear();
					client.getChannelsClient().find(chan->first)->second.getOperatorVector().clear();
					client.getChannelsClient().clear();
					client.getChannelsInvite().clear();
					return ;
				}
				else
				{
					for (std::map<int, Clients>::iterator it = clientMap.begin(); it != clientMap.end(); it++)
					{
						if (it->first != client.getFd())
						{
//client.getChannels().find(chan->first)->second.setOperator(it->second);
							break ;
						}
					}
					client.getChannelsClient().find(chan->first)->second.removeOperator(client);
				}
			}
		}
		server.getChannels().find(chan->first)->second.getClientMap().erase(ok);
		std::map<int, Clients>::iterator invitedMapIt = server.getChannels().find(chan->first)->second.getClientInvited().find(client.getFd());
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
