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
		std::map<std::string, Clients>::iterator ok = clientMap.find();
	}

	(void)client;
	(void)server;
}
