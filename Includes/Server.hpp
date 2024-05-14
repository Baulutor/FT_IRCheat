#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include "Clients.hpp"

class Clients;

class Server
{
    public:
        void setFd(int fd);
        void setAddrIp(std::string addrIp);
        int getFd() const;
        std::string getAddrIp() const;
        Server( std::string av );
        void cmdHandler(std::string cmd, Clients& client);
        std::map<std::string, Channels>& getChannels();
        // ~Server();

    private:
        int _fd;
        std::string _addrIp;
        std::map<std::string, Clients> _clients;
        std::map<std::string, Channels> _channels;
};

#endif