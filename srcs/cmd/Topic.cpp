//
// Created by dbaule on 5/28/24.
//

#include "Cmd.hpp"

#include <ctime>

void 	Topic(std::string cmd, Clients& client, Server& server)
{
	std::string channelName = &cmd[6];
	size_t forSeparate;
	forSeparate = channelName.find(" ");
	channelName = channelName.substr(0, forSeparate);
	if (channelName[channelName.size() - 1] == '\n')
		channelName = channelName.substr(0, channelName.size() - 2);
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
			sendCmd(RPL_NOTOPIC(channelName),client);
		else
		{
			sendCmd(RPL_CMD_TOPIC(client.getNickname(), channelName, channel->second.getTopic()), client); // affiche pas correctement le channelname
//			sendCmd(RPL_TOPICWHOTIME(client.getNickname(), channelName, channel->second.getTimeTopic()), client);
		}
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

//	time_t t;

	server.getChannels().find(channel->first)->second.convertTimestampToDateString();
	std::cout << channel->second.getTimeTopic() << "= CHELOU SA MERE C'est channel second la loll" << std::endl;
	server.getChannels().find(channelName)->second.setTopic(topicMessage);
	sendBrodcastChannel(RPL_CMD_TOPIC(client.getNickname(), channelName, topicMessage), channel->second);

//	std::cout << "RPL_TOPICWHOTIME = " << RPL_TOPICWHOTIME(channelName, client.getNickname(), channel->second.getTimeTopic()) << std::endl;
//	sendBrodcastChannel(RPL_TOPICWHOTIME(client.getNickname(), channelName, client.getNickname(), channel->second.getTimeTopic(), std::asctime(std::localtime(&t))), channel->second);
}

void Channels::convertTimestampToDateString()
{
	std::time_t t = std::time(NULL);

//	std::cout << "|" << std::asctime(std::localtime(&t)) << "|" << std::endl;
	std::string buf = std::asctime(std::localtime(&t));;
	buf = buf.substr(0, buf.size() - 1);
	setTimeTopic(buf);

}


