//
// Created by dbaule on 5/30/24.
//

#include "Cmd.hpp"
#include "Server.hpp"

void Quit(std::string cmd, Clients& client, Server& server)
{
	std::cout << "PASS in Quit before" << std::endl;
	std::string reason = &cmd[6];
	reason = reason.substr(0, reason.size() - 2);
	int fdCible = client.getFd();

	std::map<std::string, Channels>& servChannel = server.getChannels();
	for (std::map<std::string, Channels>::iterator itChan = servChannel.begin(); itChan != servChannel.end(); ++itChan) 
	{
		std::cout << "PASS in Quit before get client in channel" << std::endl;
		std::map<int, Clients>& clientInChan = itChan->second.getClientMap();
		for (std::map<int, Clients>::iterator itClient = clientInChan.begin(); itClient != clientInChan.end();) 
		{
			std::cout << "PASS in Quit before get the right client in channel" << std::endl;
			if (fdCible == itClient->first)
			{
				std::cout << "PASS in Quit before get operator" << std::endl;
				Clients operatorChan = itChan->second.getOperator(itClient->first);
				if (operatorChan.getFd() == fdCible && itChan->second.getClientMap().size() > 1)
				{
					std::cout << "PASS in Quit before get operator vector" << std::endl;
					if (itChan->second.getOperatorVector().size() == 1)
					{
						std::map<int, Clients>::iterator itTmp = clientInChan.begin();
						if (itTmp->first != fdCible)
						{
							itChan->second.setOperator(itTmp->second);
						}
						else 
						{
							++itTmp;
							itChan->second.setOperator(itTmp->second);
						}
						NameLstUpadte(client, itChan->second);
					}
					itChan->second.removeOperator(client);
				}
				std::cout << "PASS in Quit before sendBrodcastChannel" << std::endl;
				sendBrodcastChannel(RPL_QUIT_CHANNEL(client.getNickname(), client.getUsername(), client.getAddrIp(), itChan->first, reason), itChan->second);
				std::cout << "PASS in Quit after sendBrodcastChannel" << std::endl;
				std::cout << "itChan->second.getClientMap().size() = " << itChan->second.getClientMap().size() << std::endl;
				if (itChan->second.getClientMap().size() == 1)
				{
					std::cout << "PASS in Quit before removeOperator" << std::endl;
					itChan->second.removeOperator(client);
					std::cout << "PASS in Quit after removeOperator" << std::endl;
					sendCmd(RPL_PART(client.getNickname(), itChan->second.getName(), client.getUsername(), client.getAddrIp()), client);
					std::cout << "PASS in Quit before erase channel" << std::endl;
					server.getChannels().erase(itChan++);
					std::cout << "PASS in Quit after erase channel" << std::endl;
					if (server.getClients().size() != 1)
						++itChan;
					else
					{
						std::cout << "PASS in Quit last channel" << std::endl;
						int i = 0;
						for (std::vector<pollfd>::iterator itPollFd = server.getLstPollFd().begin(); itPollFd != server.getLstPollFd().end(); ++itPollFd) 
						{
							if (itPollFd->fd == fdCible)
							{
								server.getLstPollFd().at(i).events = 0;
								server.getLstPollFd().at(i).revents = 0;
								server.getLstPollFd().erase(itPollFd);
								std::cout << "PASS in Quit after erase pollfd" << std::endl;
								break ;
							}
							i++;
						}
						server.getClients().erase(client.getFd());
						std::cout << "PASS in Quit after erase client in server" << std::endl;
						return ;
					}
					break ;
				}
				clientInChan.erase(itClient++);
				std::cout << "PASS in Quit after erase client in channel" << std::endl;
			}
			else
				++itClient;
		}
	}
	int i = 0;
	std::cout << "PASS in Quit before erase pollfd" << std::endl;
	for (std::vector<pollfd>::iterator itPollFd = server.getLstPollFd().begin(); itPollFd != server.getLstPollFd().end(); ++itPollFd) 
	{
		if (itPollFd->fd == fdCible)
		{
			server.getLstPollFd().at(i).events = 0;
			server.getLstPollFd().at(i).revents = 0;
			server.getLstPollFd().erase(itPollFd);
			std::cout << "PASS in Quit after erase pollfd" << std::endl;
			break ;
		}
		i++;
	}
	server.getClients().erase(client.getFd());
	std::cout << "PASS in Quit after erase client in server" << std::endl;
}
