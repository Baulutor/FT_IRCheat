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
#include <poll.h>
#include <cstdio>
#include "Clients.hpp"
#include "Channels.hpp"
#include <exception>

class Channels;

class Clients;

class Server
{
    public:
        Server( std::string av, std::string av2 );
        ~Server();

        void setFd(int fd);
        void setAddrIp(std::string addrIp);
        void setDataServer();
        void setChannels(std::map<std::string, Channels> channels);
        void setClient(std::map<int, Clients> clients);
        void setPassword(std::string password);

        std::string getAddrIp() const;
        int getFd() const;
        std::map<std::string, Channels>& getChannels();
        std::map<int, Clients>& getClients();
        void cmdHandler(std::string cmd, Clients& client);
        void Pong(std::string cmd, Clients& client);
        std::string getPassword() const;

		// void	broadcast_message(const std::string &message, int sender_fd, std::vector<pollfd> &clients);

    private:
	    std::vector<pollfd> _lstPollFd;
        int _fd;
        std::string _addrIp;
        std::string _password;
        std::map<int, Clients>  _clients;
        std::map<std::string, Channels> _channels;

        // std::vector<pollfd>             _vecClient;
};

#endif