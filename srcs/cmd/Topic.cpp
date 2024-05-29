//
// Created by dbaule on 5/28/24.
//

#include "Cmd.hpp"


void 	Topic(std::string cmd, Clients& client, Server& server)
{
	std::string channelName = &cmd[6];
	size_t forSeparate;
	forSeparate = channelName.find(" ");
	channelName = channelName.substr(0, forSeparate);
	if (channelName[channelName.size() - 1] == '\n')
		channelName = channelName.substr(0, channelName.size() - 2);
	std::cout <<"["<< channelName<<"]channelName et client.getNickname()[" << client.getNickname()<<"]" << std::endl;
	if (channelName == client.getNickname())
	{
		sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), "topic"), client);
		return ;
	}
	std::map<std::string, Channels>::iterator channel = server.getChannels().find(channelName);
	if (channel == server.getChannels().end())
	{
		sendCmd(ERR_NOSUCHCHANNEL(client.getNickname(), channelName), client);
		return ;
	}
	if (cmd[6 + channelName.size() + 2] == '\0')
	{
		if (channel->second.getTopic() == "")
			sendBrodcastMSG(RPL_NOTOPIC(channelName), channel->second, client); // je veux envoyer le message dans le channel maius uniquement au client concerne !
		else
			sendBrodcastMSG(RPL_TOPIC(channelName, channel->second.getTopic()), channel->second, client); // pareil
		return ;
	}
	std::string topicMessage = &cmd[6 + channelName.size() + 2];
	topicMessage = topicMessage.substr(0, topicMessage.size() - 2);
	if (topicMessage == "\"\"")
		topicMessage = "\0";
	std::map<std::string, Clients>::iterator ClientInChannel= channel->second.getClientMap().find(client.getNickname());
	if (ClientInChannel == channel->second.getClientMap().end())
	{
		sendCmd(ERR_NOTONCHANNEL(client.getNickname(), channelName), client);
		return ;
	}

	if (channel->second.getOperator(client.getNickname()).getNickname() == "" && channel->second.getMode(channelName).find("t") != channelName.npos)
	{
		sendCmd(ERR_CHANOPRIVSNEEDED(client.getNickname(), channelName), client); // ptre mieux d'envoyer au mec uniauement mais sur le channel !
		return ;
	}
	server.getChannels().find(channelName)->second.setTopic(topicMessage);
	sendBrodcastChannel(RPL_CMD_TOPIC(client.getNickname(), channelName, topicMessage), channel->second);
}
