/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibernar <nibernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:36:58 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/30 13:17:13 by nibernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Clients.hpp"
#include <sstream>

Clients::Clients()
{
    _fd = -1;
    _addrIp = "";
    _nickname = "";
    _username = "";
    _pass = "";
    _isRegistered = false;
    bzero(_buffer, 512);
    bzero(_bufferTmp, 512);
    // std::cout << "Clients constructor created" << std::endl;
}

Clients::~Clients()
{
    // std::cout << "Clients destructor created" << std::endl;
    // std::cout << "fd : " << _fd << std::endl;
    // std::cout << "name : " << _nickname << std::endl;
    // delete[] _buffer;
    // delete[] _bufferTmp;
}

// Getters

int Clients::getFd() const {return (_fd);}

std::string Clients::getAddrIp() const {return (_addrIp);}

std::string Clients::getNickname() const {return (_nickname);}

std::string Clients::getUsername() const {return (_username);}

std::string Clients::getPass() const {return (_pass);}

std::map<std::string, Channels>& Clients::getChannels() {return (_channels);}

std::map<std::string, Channels> Clients::getChannelsInvite() {return (_channelsInvite);}

bool Clients::getIsRegistered() const {return (_isRegistered);}

char* Clients::getBuffer() {return (_buffer);}

char* Clients::getBufferTmp() {return (_bufferTmp);}

// Setters

void Clients::setFd(int fd) {_fd = fd;}

void Clients::setAddrIp(std::string addrIp) {_addrIp = addrIp;}

void Clients::setNickname(std::string nickname) {_nickname = nickname;}

void Clients::setUsername(std::string username) {_username = username;}

void Clients::setPass(std::string pass) {_pass = pass;}

void Clients::setIsRegistered(bool isRegistered) {_isRegistered = isRegistered;}

void Clients::setBuffer(char* buffer)
{
    if (_buffer[0] == '\0')
        strcpy(_buffer, buffer);
    else
        bzero(_buffer, 512);
}

void Clients::setBufferTmp(char* bufferTmp)
{
    if (bufferTmp != NULL)
        strcat(_bufferTmp, bufferTmp);
    else
        bzero(_bufferTmp, 512);
}

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
        // std::cout << "caca ici" << std::endl;
        if (tokens[i].find("PASS") != std::string::npos && PASS < 0)
        {
            if (tokens[i + 1] != server.getPassword())
            {
                std::cout << "ERR_PASSWDMISMATCH = " << ERR_PASSWDMISMATCH(tokens[i + 1]) << std::endl;
                sendCmd(ERR_PASSWDMISMATCH(tokens[i + 1]), *this);
                _isRegistered = false;
                PASS = i;
                // server.getClients().erase(getFd());
                return (false);
            }
            PASS = i;
            setPass(tokens[i + 1]);
        }
    }
    for (size_t i = 0; i < tokens.size(); ++i)
    {
        if (tokens[i].find("USER") != std::string::npos && USER < 0)
        {
            // if (_pass == "") {
            //     // sendCmd("ERROR: must confirm password first\r\n", *this);
            //     return (false);
            // }
            USER = i;
            setUsername(tokens[i + 1]);
        }
    }
    static bool nickname_used = false;
    static std::string falseNickname = "";
    for (size_t i = 0; i < tokens.size(); ++i)
    {
        // std::cout << "caca ici" << std::endl;
        if (tokens[i].find("NICK") != std::string::npos && NICK < 0)
        {
            for (std::map<int, Clients>::iterator it = server.getClients().begin(); it != server.getClients().end(); it++)
            {
                if (it->second.getNickname() == tokens[i + 1])
                {
                    nickname_used = true;
                    falseNickname = tokens[i + 1];
                    sendCmd(RPL_ERROR_NICKNAME_IN_USE(getNickname(), falseNickname), *this);
                    return (false);
                }
            }
            if (nickname_used == true)
            {
                std::cout << "RPL_CMD_NICK = " << RPL_CMD_NICK(falseNickname, getUsername(), getAddrIp(), tokens[i + 1]) << std::endl;
                sendCmd(RPL_CMD_NICK(falseNickname, getUsername(), getAddrIp(), tokens[i + 1]), *this);
            }
            NICK = i;
            setNickname(tokens[i + 1]);
        }
    }
    std::cout << "USER : " << USER << std::endl;
    std::cout << "NICK : " << NICK << std::endl;
    std::cout << "PASS : " << PASS << std::endl;
    if (((USER > -1 || NICK > -1) || (USER > -1 && NICK > -1)) && PASS == -1)
    {
        std::cout << "ERR_PASSWDMISMATCH = " << ERR_PASSWDMISMATCH(getNickname()) << std::endl;
        sendCmd(ERR_PASSWDMISMATCH(getNickname()), *this);
        _isRegistered = false;
        // PASS = i;
        PASS = -1; 
        NICK = -1;
        USER = -1;
        // server.getClients().erase(getFd());
        return (true);
    }
    if (PASS > -1 && NICK > -1 && USER > -1)
    {
        sendCmd(RPL_MOTD_START(getNickname()), *this);
        sendCmd(RPL_MOTD_MSG(getNickname(), "Welcome to the FT_IRCheat"), *this);
        sendCmd(RPL_MOTD_END(getNickname()), *this);
        setAddrIp(server.getAddrIp());
        _isRegistered = true;
        PASS = -1;
        NICK = -1;
        USER = -1;
        return (true);
    }
    return (false);
}
