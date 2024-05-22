#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <map>
#include "Clients.hpp"

class Clients;

class Server
{
    public:
        Server( std::string av );
        ~Server();

        void setFd(int fd);
        void setAddrIp(std::string addrIp);
        void setDataServer();
        void setChannels(std::map<std::string, Channels> channels);
        void setClient(std::map<std::string, Clients> clients);

        std::string getAddrIp() const;
        int getFd() const;
        std::map<std::string, Channels>& getChannels();
        std::map<std::string, Clients>& getClients();
        void cmdHandler(std::string cmd, Clients& client);
        void Pong(std::string cmd, Clients& client);

    private:
        int _fd;
        std::string _addrIp;
        std::map<std::string, Clients> _clients;
        std::map<std::string, Channels> _channels;
};

#endif