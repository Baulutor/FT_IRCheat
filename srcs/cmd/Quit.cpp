//
// Created by dbaule on 5/30/24.
//

#include "Cmd.hpp"
#include "Server.hpp"

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

	std::string reason = &cmd[6];
	reason = reason.substr(0, reason.size() - 2);
	std::cout << "cmd: [" << reason << "]" << std::endl;


	// server.getChannels();
	std::map<std::string, Channels> servChannel = server.getChannels();

	// le seul truc que je veux utiliser du vecteur de client le reste ca va etre server
	std::map<std::string, Channels> clientChannel = client.getChannelsClient();


	// BOUCLE de for du nombre de channel dont le client est inclus !
	for (std::map<std::string, Channels>::iterator it = clientChannel.begin(); it != clientChannel.end(); it++)
	{
		std::vector<Clients> vecOperator = servChannel.find(it->first)->second.getOperatorVector();
		//servChannel.find(it->first)
		std::map<std::string, Channels>::iterator iterServChannel = servChannel.find(it->first);

		// servChannel.find(it->first)->second.getClientMap();
		std::map<int, Clients> serverMapClients = servChannel.find(it->first)->second.getClientMap();

		if (vecOperator.size() == 1)
		{
			if (serverMapClients.size() > 1)
			{
				if (vecOperator.begin()->getFd() == client.getFd()) // ca veut dire si le client est l'operateur !
				{
					if (serverMapClients.begin()->second.getFd() == client.getFd())
					{
						std::map<int, Clients>::iterator forOpe =  serverMapClients.begin()++;
						iterServChannel->second.setOperator(forOpe->second);
					}
					else
						iterServChannel->second.setOperator(serverMapClients.begin()->second);
					iterServChannel->second.removeOperator(client);
				}
			}
		}
		else
		{
			// juste supprimer le client du channel (serv)! et enlever de son role d'operateur si jamais il l'etais !
			for (std::vector<Clients>::iterator ite = vecOperator.begin(); ite != vecOperator.end(); ite++)
			{
				if (ite->getFd() == client.getFd())
					iterServChannel->second.removeOperator(client);
			}
		}
//		NameLstUpadte(client, iterServChannel->second);
		sendBrodcastChannel(RPL_QUIT_CHANNEL(client.getNickname(), client.getUsername(), client.getAddrIp(), it->first, reason), it->second);
		if (serverMapClients.size() == 1)
		{
			std::cout << "serverMapClients get clear  " << std::endl;
			sendBrodcastChannel("quit avec la raison suivant : " + reason, iterServChannel->second);
			client.getChannelsClient().find(it->first)->second.getClientMap().clear();
			server.getChannels().find(it->first)->second.getClientMap().clear();
			server.getChannels().erase(server.getChannels().find(it->first));
			continue ;
		}
		else
		{
			std::cout << "JE vais supprimer le frero du channel donc la taille avant: " << server.getChannels().find(it->first)->second.getClientMap().size() << std::endl;
			server.getChannels().find(it->first)->second.getClientMap().erase(serverMapClients.find(client.getFd()));
			std::cout << "APRERERERRSRRERRSRRERRSRRERSRERRSRERRSRERSRRERSRERRSRFUJHFKLSIHDGFVJHDBKNCSIFUHHBJK" << std::endl;
			client.getChannelsClient().find(it->first)->second.getClientMap().erase(client.getChannelsClient().find(it->first)->second.getClientMap().find(client.getFd()));
			std::cout << "taille apres: " << server.getChannels().find(it->first)->second.getClientMap().size() << std::endl;
		}
	}
	sendBrodcastServer("ERROR: " + client.getNickname() + " Disconnected from server\r\n", server);

	std::vector<pollfd> lstPollFdVec = server.getLstPollFd();
	for (size_t i = 0; i < lstPollFdVec.size(); i++)
	{
		if (client.getFd() == lstPollFdVec[i].fd)
		{
			server.getLstPollFd().at(i).revents = 0;
			server.getLstPollFd().at(i).events = 0;
			std::cerr << "close pollfd= " << close(server.getLstPollFd().at(i).fd) << std::endl;
			server.getLstPollFd().erase(server.getLstPollFd().begin() + i);
			break ;
		}
	}

	int buf = client.getFd();
	client.setFd(-1);
	server.getClients().erase(server.getClients().find(buf));
}