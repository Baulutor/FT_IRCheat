//
// Created by dbaule on 5/30/24.
//

#include "Cmd.hpp"
#include "Server.hpp"

// void Quit(std::string cmd, Clients& client, Server& server)
// {

// 	std::string reason = &cmd[6];
// 	reason = reason.substr(0, reason.size() - 2);

// 	// server.getChannels();
// 	std::map<std::string, Channels>& servChannel = server.getChannels();

// 	// le seul truc que je veux utiliser du vecteur de client le reste ca va etre server
// 	std::map<std::string, Channels>& clientChannel = client.getChannelsClient();


// 	// BOUCLE de for du nombre de channel dont le client est inclus !
// 	for (std::map<std::string, Channels>::iterator it = clientChannel.begin(); it != clientChannel.end(); it++)
// 	{
// 		std::vector<Clients> vecOperator = servChannel.find(it->first)->second.getOperatorVector();
// 		//servChannel.find(it->first)
// 		std::map<std::string, Channels>::iterator iterServChannel = servChannel.find(it->first);

// 		// servChannel.find(it->first)->second.getClientMap();
// 		std::map<int, Clients> serverMapClients = servChannel.find(it->first)->second.getClientMap();

// 		if (vecOperator.size() == 1)
// 		{
// 			if (serverMapClients.size() > 1)
// 			{
// 				if (vecOperator.begin()->getFd() == client.getFd()) // ca veut dire si le client est l'operateur !
// 				{
// 					if (serverMapClients.begin()->second.getFd() == client.getFd())
// 					{
// 						std::map<int, Clients>::iterator forOpe =  serverMapClients.begin()++;
// 						iterServChannel->second.setOperator(forOpe->second);
// 					}
// 					else
// 						iterServChannel->second.setOperator(serverMapClients.begin()->second);
// 					iterServChannel->second.removeOperator(client);
// 				}
// 			}
// 		}
// 		else
// 		{
// 			// juste supprimer le client du channel (serv)! et enlever de son role d'operateur si jamais il l'etais !
// 			for (std::vector<Clients>::iterator ite = vecOperator.begin(); ite != vecOperator.end(); ite++)
// 			{
// 				if (ite->getFd() == client.getFd())
// 					iterServChannel->second.removeOperator(client);
// 			}
// 		}
// //		NameLstUpadte(client, iterServChannel->second);
// 		sendBrodcastChannel(RPL_QUIT_CHANNEL(client.getNickname(), client.getUsername(), client.getAddrIp(), it->first, reason), it->second);
// 		if (serverMapClients.size() == 1)
// 		{
// 			std::cout << "serverMapClients get clear  " << std::endl;
// 			sendBrodcastChannel("quit avec la raison suivant : " + reason, iterServChannel->second);
// 			server.getChannels().find(it->first)->second.getClientMap().clear();
// 			server.getChannels().erase(server.getChannels().find(it->first));
// 			continue ;
// 		}
// 		else
// 		{
// 			std::cout << "chelou ce channel la: " << server.getChannels().find(it->first)->first << ", et le frero: " << serverMapClients.find(client.getFd())->first << std::endl;
// 			server.getChannels().find(it->first)->second.getClientMap().erase(serverMapClients.find(client.getFd()));
// 		}
// 	}
// 	std::cerr << "le fd du client A TEJ CE CHIEN: " << client.getFd() << ", et son nom de pute ! " << client.getNickname() << std::endl;
// //	clearForAllUser(server, client.getFd());
// 	client.getChannelsClient().clear();
// 	sendBrodcastServer("ERROR: " + client.getNickname() + " Disconnected from server\r\n", server);

// 	std::vector<pollfd> lstPollFdVec = server.getLstPollFd();
// 	for (size_t i = 0; i < lstPollFdVec.size(); i++)
// 	{
// 		if (client.getFd() == lstPollFdVec[i].fd)
// 		{
// 			server.getLstPollFd().at(i).revents = 0;
// 			server.getLstPollFd().at(i).events = 0;
// 			close(server.getLstPollFd().at(i).fd);
// 			server.getLstPollFd().erase(server.getLstPollFd().begin() + i);
// 			break ;
// 		}
// 	}
// 	server.setQuitFd(client.getFd());
// }


void Quit(std::string cmd, Clients& client, Server& server)
{
	std::string reason = &cmd[6];
	reason = reason.substr(0, reason.size() - 2);
	std::string cible = client.getNickname();
	//int fdCible = client.getFd();

	// server.getChannels();
	std::map<std::string, Channels>& servChannel = server.getChannels();
	std::cout << "caca" << std::endl;
	for (std::map<std::string, Channels>::iterator itChan = servChannel.begin(); itChan != servChannel.end(); ++itChan) 
	{
		std::map<int, Clients>& clientInChan = itChan->second.getClientMap();
		std::cerr << "premier client: " << itChan->second.getClientMap().begin()->first << ", dans le channel: " << itChan->first << std::endl;
		for (std::map<int, Clients>::iterator itClient = clientInChan.begin(); itClient != clientInChan.end(); itClient++) 
		{
			if (cible == itClient->second.getNickname()) {
				sendBrodcastChannel(RPL_QUIT_CHANNEL(client.getNickname(), client.getUsername(), client.getAddrIp(), itChan->first, reason), itChan->second);
				clientInChan.erase(itClient++);
			}
		}
	}
	// int i = 0;
	// for (std::vector<pollfd>::iterator itPollFd = server.getLstPollFd().begin(); itPollFd != server.getLstPollFd().end(); ++itPollFd) 
	// {
	// 	if (itPollFd->fd == fdCible)
	// 	{
	// 		server.getLstPollFd().at(i).events = 0;
	// 		server.getLstPollFd().at(i).revents = 0;
	// 		server.getLstPollFd().erase(itPollFd);
	// 	}
	// 	i++;
	// }
}


// 	// le seul truc que je veux utiliser du vecteur de client le reste ca va etre server
// 	std::map<std::string, Channels>& clientChannel = client.getChannelsClient();


// 	// BOUCLE de for du nombre de channel dont le client est inclus !
// 	for (std::map<std::string, Channels>::iterator it = clientChannel.begin(); it != clientChannel.end(); it++)
// 	{
// 		std::vector<Clients> vecOperator = servChannel.find(it->first)->second.getOperatorVector();
// 		//servChannel.find(it->first)
// 		std::map<std::string, Channels>::iterator iterServChannel = servChannel.find(it->first);

// 		// servChannel.find(it->first)->second.getClientMap();
// 		std::map<int, Clients> serverMapClients = servChannel.find(it->first)->second.getClientMap();

// 		if (vecOperator.size() == 1)
// 		{
// 			if (serverMapClients.size() > 1)
// 			{
// 				if (vecOperator.begin()->getFd() == client.getFd()) // ca veut dire si le client est l'operateur !
// 				{
// 					if (serverMapClients.begin()->second.getFd() == client.getFd())
// 					{
// 						std::map<int, Clients>::iterator forOpe =  serverMapClients.begin()++;
// 						iterServChannel->second.setOperator(forOpe->second);
// 					}
// 					else
// 						iterServChannel->second.setOperator(serverMapClients.begin()->second);
// 					iterServChannel->second.removeOperator(client);
// 				}
// 			}
// 		}
// 		else
// 		{
// 			// juste supprimer le client du channel (serv)! et enlever de son role d'operateur si jamais il l'etais !
// 			for (std::vector<Clients>::iterator ite = vecOperator.begin(); ite != vecOperator.end(); ite++)
// 			{
// 				if (ite->getFd() == client.getFd())
// 					iterServChannel->second.removeOperator(client);
// 			}
// 		}
// //		NameLstUpadte(client, iterServChannel->second);
// 		if (serverMapClients.size() == 1)
// 		{
// 			std::cout << "serverMapClients get clear  " << std::endl;
// 			sendBrodcastChannel("quit avec la raison suivant : " + reason, iterServChannel->second);
// 			server.getChannels().find(it->first)->second.getClientMap().clear();
// 			server.getChannels().erase(server.getChannels().find(it->first));
// 			continue ;
// 		}
// 		else
// 		{
// 			std::cout << "chelou ce channel la: " << server.getChannels().find(it->first)->first << ", et le frero: " << serverMapClients.find(client.getFd())->first << std::endl;
// 			server.getChannels().find(it->first)->second.getClientMap().erase(serverMapClients.find(client.getFd()));
// 		}
// 	}
// 	std::cerr << "le fd du client A TEJ CE CHIEN: " << client.getFd() << ", et son nom de pute ! " << client.getNickname() << std::endl;
// //	clearForAllUser(server, client.getFd());
// 	client.getChannelsClient().clear();
// 	sendBrodcastServer("ERROR: " + client.getNickname() + " Disconnected from server\r\n", server);

// 	std::vector<pollfd> lstPollFdVec = server.getLstPollFd();
// 	for (size_t i = 0; i < lstPollFdVec.size(); i++)
// 	{
// 		if (client.getFd() == lstPollFdVec[i].fd)
// 		{
// 			server.getLstPollFd().at(i).revents = 0;
// 			server.getLstPollFd().at(i).events = 0;
// 			close(server.getLstPollFd().at(i).fd);
// 			server.getLstPollFd().erase(server.getLstPollFd().begin() + i);
// 			break ;
// 		}
// 	}
// 	server.setQuitFd(client.getFd());
// }
