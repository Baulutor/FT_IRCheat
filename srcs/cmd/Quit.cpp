//
// Created by dbaule on 5/30/24.
//

#include "Cmd.hpp"

#include "Cmd.hpp"

//void Quit(std::string cmd, Clients& client, Server& server)
//{
//
//	std::cout << "["<< cmd << "]" <<  std::endl;
//
//	// CLIENT
//	std::map<std::string, Channels> channelsClient = client.getChannelsClient();
//	std::map<std::string, Channels>::iterator itClientChan = channelsClient.begin();
//
//	//SERVER
//	std::map<std::string, Channels> channelsServ = server.getChannels();
//
//	// POLLFD SERVER
//	std::vector<pollfd> pollFdServ = server.getLstPollFd();
//
//	for (; itClientChan != channelsClient.end(); itClientChan++)
//	{
//
//		//Client iterator or Map
//		std::map<int, Clients> clientMap = itClientChan->second.getClientMap();
////		std::map<int, Clients> clientInvitedMap = itClientChan->second.getClientInvited();
////		std::map<int, Clients>::iterator erasingClientChannel = clientMap.find(client.getFd());
//
//		std::vector <Clients> opeVec = itClientChan->second.getOperatorVector();
//
//		std::cout << "chan->first: " << itClientChan->first << " , valeur de opeVec.size(): " << opeVec.size() << std::endl;
//		if (opeVec.size() == 1)
//		{
//			if (opeVec.begin()->getFd() == client.getFd())
//			{
//				if (channelsServ.find(itClientChan->first)->second.getClientMap().size() == 1) {
//					close(client.getFd());
//					client.getAddrIp().clear();
//					for (size_t i = 0; i < pollFdServ.size(); i++)
//					{
//						if (client.getFd() == pollFdServ[i].fd)
//						{
//							close(pollFdServ[i].fd);
//							server.getChannels().find(itClientChan->first)->second.getClientMap().clear(); // pas sur
//							clientMap.clear();
//							server.getChannels().erase(server.getChannels().find(itClientChan->first));
//							server.getLstPollFd().at(i).revents = 0;
//							server.getLstPollFd().at(i).events = 0;
//							server.getLstPollFd().erase(server.getLstPollFd().begin() + 1);
//							return;
//						}
//					}
//				}
//				else
//				{
//					if (opeVec.begin()->getFd() == itClientChan->second.getClientMap().find(client.getFd())->second.getFd())
//					{
//						for (std::map<int, Clients>::iterator it = channelsServ.find(itClientChan->first)->second.getClientMap().begin(); it != channelsServ.find(itClientChan->first)->second.getClientMap().end(); it++)
//						{
//							std::cout << "est ce que je suis la ??" << std::endl;
//							if (it->first != client.getFd())
//							{
//								server.getChannels().find(itClientChan->first)->second.setOperator(server.getChannels().find(itClientChan->first)->second.getClientMap().find(it->first)->second);
//								std::cout << "nombre d'ope apres avoir rajoute !: " << opeVec.size() << std::endl;
//								break;
//							}
//						}
//						server.getChannels().find(itClientChan->first)->second.removeOperator(client);
//					}
//				}
//			}
//		}
//		// supprimer le client de chaque channel dont il fait partie, virer de la map des invite s'il y es dans la client map
//		NameLstUpadte(client, itClientChan->second);
//	}
//}

void Quit(std::string cmd, Clients& client, Server& server)
{


	(void)cmd;
	(void)client;
	(void)server;
}