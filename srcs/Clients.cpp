/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:36:58 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/08 16:46:22 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Clients.hpp"

Clients::Clients()
{
    std::cout << "Clients constructor created" << std::endl;
}

Clients::~Clients()
{
    std::cout << "Clients destructor created" << std::endl;
}

// Getters

int Clients::getFd() const {return (_fd);}

std::string Clients::getAddrIp() const {return (_addrIp);}

std::string Clients::getNickname() const {return (_nickname);}

std::string Clients::getUsername() const {return (_username);}

std::string Clients::getPass() const {return (_pass);}

// Setters

void Clients::setFd(int fd) {_fd = fd;}

void Clients::setAddrIp(std::string addrIp) {_addrIp = addrIp;}

void Clients::setNickname(std::string nickname) {_nickname = nickname;}

void Clients::setUsername(std::string username) {_username = username;}

void Clients::setPass(std::string pass) {_pass = pass;}

// Print info

void Clients::printInfo()
{
    std::cout << "Nickname : " << _nickname << std::endl;
    std::cout << "Username" << _username << std::endl;
    std::cout << "Pass : " << _pass << std::endl;
    std::cout << "fd : " << _fd << std::endl;
    std::cout << "Ip Address : " << _addrIp << std::endl;
}

bool Clients::initClients(std::string line)
{
    static int PASS = -1;
    static int NICK = -1;
    static int USER = -1;

    if (PASS < 0)
        PASS = line.find("PASS");
    if (NICK < 0)
        NICK = line.find("NICK");
    if (USER < 0)
        USER = line.find("USER");
    std::cout << "line in initClients : |" << line << "|" << std::endl;
    std::cout << "ID PASS : " << PASS << std::endl;
    std::cout << "ID NICK : " << NICK << std::endl;
    std::cout << "ID USER : " << USER << std::endl;
    if (PASS > -1)
    {
        std::cout << "char PASS : " << line[PASS] << std::endl;
        std::cout << "char PASS + 5 : " << line[PASS + 5] << std::endl;
        line = (PASS + 5);
        std::cout << "char PASS end : " << line[line.find('\r') - 1] << std::endl;
    }
    if (NICK > -1)
    {
        std::cout << "char NICK : " << line[NICK] << std::endl;
        std::cout << "char NICK + 5 : " << line[NICK + 5] << std::endl;
        line = (NICK + 5);
        std::cout << "char NICK end : " << line[line.find('\r') - 1] << std::endl;
    }
    if (USER > -1)
    {
        std::cout << "char USER : " << line[USER] << std::endl;
        std::cout << "char USER + 5 : " << line[USER + 5] << std::endl;
        line = (USER + 5);
        std::cout << "char USER end : " << line[line.find('\r') - 1] << std::endl;
    }
    if (PASS > -1 && NICK > -1 && USER > -1)
        return (true);
    return (false);
}
