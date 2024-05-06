#ifndef CLIENTS_HPP
#define CLIENTS_HPP

#include <string>
#include <cstdlib>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class Clients
{
public:
    void setFd(int fd);
    void setAddrIp(std::string addrIp);
    int getFd() const;
    std::string getAddrIp() const;
    Clients();
private:
    int _fd;
    std::string _addrIp;
};

#endif