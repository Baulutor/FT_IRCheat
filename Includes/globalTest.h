//
// Created by dbaule on 5/27/24.
//

#ifndef GLOBALTEST_H
#define GLOBALTEST_H

typedef struct forFree
{
	std::vector<pollfd> *_lstPollFdPTR;
	std::string *_addrIpPtr;
	std::string *_passwordPtr;
	std::map<int, Clients>  *_clientsPtr;
	std::map<std::string, Channels> *_channelsPtr;

	Server	*toFree;

}	forFree;


extern forFree clearGlob;

#endif
