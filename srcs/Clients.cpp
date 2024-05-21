/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:36:58 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/21 19:49:26 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Clients.hpp"
#include <sstream>

Clients::Clients()
{
    //std::cout << "Clients constructor created" << std::endl;
}

Clients::~Clients()
{
    //std::cout << "Clients destructor created" << std::endl;
}

// Getters

int Clients::getFd() const {return (_fd);}

std::string Clients::getAddrIp() const {return (_addrIp);}

std::string Clients::getNickname() const {return (_nickname);}

std::string Clients::getUsername() const {return (_username);}

std::string Clients::getPass() const {return (_pass);}

std::map<std::string, Channels>& Clients::getChannels() {return (_channels);}

std::map<std::string, Channels> Clients::getChannelsInvite() {return (_channelsInvite);}

// Setters

void Clients::setFd(int fd) {_fd = fd;}

void Clients::setAddrIp(std::string addrIp) {_addrIp = addrIp;}

void Clients::setNickname(std::string nickname) {_nickname = nickname;}

void Clients::setUsername(std::string username) {_username = username;}

void Clients::setPass(std::string pass) {_pass = pass;}

// Print info


void Clients::setPollFd(pollfd newPoll)
{
	this->pollClient = newPoll;
}


pollfd	Clients::getPollFd()
{
    return (this->pollClient);
}

void Clients::printInfo()
{
    std::cout << "Nickname : " << _nickname << std::endl;
    std::cout << "Username : " << _username << std::endl;
    std::cout << "Pass : " << _pass << std::endl;
    std::cout << "fd : " << _fd << std::endl;
    std::cout << "Ip Address : " << _addrIp << std::endl;
}

void Clients::printChannels()
{
    std::cout << "Channels size : " << _channels.size() << std::endl;
    for (std::map<std::string, Channels>::iterator it = _channels.begin(); it != _channels.end(); ++it)
    {
        std::cout << "Channel : " << it->first << std::endl;
    }
}

bool Clients::initClients(std::string line, Server &server)
{
    static int PASS = -1;
    static int NICK = -1;
    static int USER = -1;

    // Split line on \r and \n, then split by the first space in each part
    std::vector<std::string> tokens;
    std::istringstream iss(line);
    std::string part;

    while (std::getline(iss, part, '\n'))
	{
        size_t pos = part.find('\r');
        if (pos != std::string::npos) {
            part = part.substr(0, pos);
        }
        // Check if the line contains "USER" and split all spaces if it does
        if (part.find("USER") != std::string::npos)
		{
            std::istringstream userStream(part);
            std::string userToken;
            while (userStream >> userToken)
			{
                tokens.push_back(userToken);
            }
        } else {
            // Otherwise, split only at the first space
            pos = part.find(' ');
            if (pos != std::string::npos)
			{
                tokens.push_back(part.substr(0, pos)); // Before the space
                tokens.push_back(part.substr(pos + 1)); // After the space
            } else
			{
                tokens.push_back(part); // No space found, push the whole part
            }
        }
    }

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        if (tokens[i].find("PASS") != std::string::npos && PASS < 0)
        {
            PASS = i;
            setPass(tokens[i + 1]);
            if (getPass() != server.getPassword())
                throw std::exception();
        }
        if (tokens[i].find("NICK") != std::string::npos && NICK < 0)
        {
            for (std::map<int, Clients>::iterator it = server.getClients().begin(); it != server.getClients().end(); it++)
            {
                if (it->second.getNickname() == tokens[i + 1])
                {
                    setNickname(tokens[i + 1] + "_");
                    NICK = i;
                    break ;
                }
            }
            NICK = i;
            setNickname(tokens[i + 1]);
        }
        if (tokens[i].find("USER") != std::string::npos && USER < 0)
        {
            USER = i;
            setUsername(tokens[i + 1]);
        }
    }
    if (PASS > -1 && NICK > -1 && USER > -1)
    {
        sendCmd(RPL_MOTD_START(getNickname()), *this);
        sendCmd(RPL_MOTD_MSG(getNickname(), "Welcome to the FT_IRCheat"), *this);
        sendCmd(RPL_MOTD_END(getNickname()), *this);
        PASS = -1;
        NICK = -1;server
    }
    return (false);
}
