/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:36:58 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/06 15:58:23 by bfaure           ###   ########.fr       */
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

int Clients::getFd() const
{
    return this->_fd;
}

std::string Clients::getAddrIp() const
{
    return this->_addrIp;
}

std::string Clients::getNick() const
{
    return (_nick);
}

std::string Clients::getPass() const
{
    return (_pass);
}

void Clients::setFd(int fd)
{
    this->_fd = fd;
}

void Clients::setAddrIp(std::string addrIp)
{
    this->_addrIp = addrIp;
}

void Clients::setNick(std::string nick)
{
    _nick = nick;
}

void Clients::setPass(std::string pass)
{
    _pass = pass;
}

void Clients::printInfo()
{
    std::cout << "Name : " << _nick << std::endl;
    std::cout << "Pass : " << _pass << std::endl;
    std::cout << "fd : " << _fd << std::endl;
    std::cout << "Ip Address : " << _addrIp << std::endl;
}
