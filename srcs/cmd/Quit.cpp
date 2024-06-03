//
// Created by dbaule on 5/30/24.
//

#include "Cmd.hpp"
#include "Server.hpp"

void clearForAllUser(Server &server, int clientFd);

void Quit(std::string cmd, Clients& client, Server& server)
{

	std::string reason = &cmd[6];
	reason = reason.substr(0, reason.size() - 2);

	// server.getChannels();
	std::map<std::string, Channels>& servChannel = server.getChannels();

	// le seul truc que je veux utiliser du vecteur de client le reste ca va etre server
	std::map<std::string, Channels>& clientChannel = client.getChannelsClient();


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
			server.getChannels().find(it->first)->second.getClientMap().clear();
			server.getChannels().erase(server.getChannels().find(it->first));
			continue ;
		}
		else
		{
			std::cout << "chelou ce channel la: " << server.getChannels().find(it->first)->first << ", et le frero: " << serverMapClients.find(client.getFd())->first << std::endl;
			server.getChannels().find(it->first)->second.getClientMap().erase(serverMapClients.find(client.getFd()));
		}
	}
	std::cerr << "le fd du client A TEJ CE CHIEN: " << client.getFd() << ", et son nom de pute ! " << client.getNickname() << std::endl;
//	clearForAllUser(server, client.getFd());
	client.getChannelsClient().clear();
	sendBrodcastServer("ERROR: " + client.getNickname() + " Disconnected from server\r\n", server);

	std::vector<pollfd> lstPollFdVec = server.getLstPollFd();
	for (size_t i = 0; i < lstPollFdVec.size(); i++)
	{
		if (client.getFd() == lstPollFdVec[i].fd)
		{
			server.getLstPollFd().at(i).revents = 0;
			server.getLstPollFd().at(i).events = 0;
			close(server.getLstPollFd().at(i).fd);
			server.getLstPollFd().erase(server.getLstPollFd().begin() + i);
			break ;
		}
	}
	server.setQuitFd(client.getFd());
//	client.setFd(-1);
//	server.getClients().erase(server.getClients().find(client.getFd()));



//	std::map<int, Clients> servClient = server.getClients();
//	for (std::map<int, Clients>::iterator it = servClient.begin(); servClient.end() != it; it++)
//	{
//		std::cerr << std::endl << "fd du client: " << it->first << ", mon nom est " << it->second.getNickname() << std::endl << std::endl;
//		std::map<std::string, Channels> channelsClient = it->second.getChannelsClient();
//		for (std::map<std::string, Channels>::iterator ite = channelsClient.begin(); ite != channelsClient.end(); ite++)
//		{
//			std::map<int, Clients> dansleclientChannel = ite->second.getClientMap();
//			for (std::map<int, Clients>::iterator iter = dansleclientChannel.begin(); dansleclientChannel.end() != iter; iter++)
//			{
//				std::cerr << "je suis: " << iter->second.getNickname() << " avec le fd " << iter->first << ", dans le channel:" << ite->first << std::endl;
//			}
//		}
//	}

}

//void clearForAllUser(Server &server, int clientFd)
//{
//	std::map<int, Clients> servClient = server.getClients();
//	for (std::map<int, Clients>::iterator it = servClient.begin(); servClient.end() != it; it++)
//	{
//		if (it->first != clientFd)
//		{
//			std::map<std::string, Channels> channelsClient = it->second.getChannelsClient();
//			for (std::map<std::string, Channels>::iterator ite = channelsClient.begin(); ite != channelsClient.end(); ite++)
//			{
//				std::map<int, Clients>::iterator test = ite->second.getClientMap().find(clientFd);
//				if (test != ite->second.getClientMap().end())
//					server.getClients().find(it->first)->second.getChannelsClient().find(ite->first)->second.getClientMap().erase(test);
//			}
//		}
//	}
//}