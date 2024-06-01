/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:44:51 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/07 11:44:51 by bfaure           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Channels.hpp"
#include <algorithm>
#include <map>

Channels::Channels() {}

Channels::Channels(std::string name, Clients& op)
{
    _name = name;
    _operator.push_back(op);
    _topic = "";
	_timeTopic = "";
    _password = " ";
    _nbClients = 0;
    _nbMaxClients = 0;
    _id = -42;
    _mode.insert(std::make_pair(_id, "t"));
    _mode.insert(std::make_pair(op.getFd(), "o"));
}

Channels::~Channels() {}

// Getter

std::string Channels::getName() const {return (_name);}

std::string Channels::getTopic() const {return (_topic);}

std::string Channels::getTimeTopic() const {return (_timeTopic);}

std::map<int, Clients>& Channels::getClientMap() {return (_clients);}

std::map<int, Clients>& Channels::getClientInvited() {return (_inviteClient);}

std::vector<Clients>& Channels::getOperatorVector() {return (_operator);}

std::string Channels::getPassword() const {return (_password);}

std::string Channels::getMode(int target) const {return (_mode.find(target)->second);}

int Channels::getLimit() const {return (_nbMaxClients);}

int Channels::getNbClients() const {return (_nbClients);}

Clients Channels::getOperator(int target) const
{
    // std::cout << "Channels operator : " << _operator.size() << std::endl;
    for (size_t i = 0; i < _operator.size(); i++)
    {
        if (_operator[i].getFd() == target)
            return (_operator[i]);
    }
    return (Clients());
}

// Setter

void Channels::setName(std::string name) {_name = name;}

void Channels::setTopic(std::string topic) {_topic = topic;}

void Channels::setTimeTopic(std::string timeTopic) {_timeTopic = timeTopic;}

void Channels::setOperator(Clients& op) {_operator.push_back(op);}

void Channels::setPassword(std::string password) {_password = password;}

void Channels::setMode(int target, std::string mode)
{
    std::cout << "setMode Mode = " << mode << std::endl;
    std::map<int, std::string>::iterator it = _mode.find(target);
    if (it != _mode.end())
        it->second += mode;
    else
        _mode.insert(std::make_pair(target, mode));
}

void Channels::setLimit(int limit) {_nbMaxClients = limit;}

void Channels::setClients(std::map<int, Clients> clients) {_clients = clients;}

void Channels::incrementNbClients() {_nbClients++;}

// Remover

void Channels::removeMode(int target, std::string mode)
{
    std::cout << "removeMode Mode = " << mode << std::endl;
    std::map<int, std::string>::iterator it = _mode.find(target);
    if (it != _mode.end())
    {
        if (it->second.length() <= 1)  // Si la chaîne est vide ou contient une seule valeur
            it->second = "";           // Remplacer par une chaîne vide
        else
            it->second.erase(std::remove(it->second.begin(), it->second.end(), mode[0]), it->second.end());  // Effacer le caractère spécifié
    }
}

void Channels::removeOperator(Clients& op)
{
    for (size_t i = 0; i < _operator.size(); i++)
    {
        if (_operator[i].getFd() == op.getFd())
            _operator.erase(_operator.begin() + i);
    }
}
void Channels::ClientInvite(std::map<int, Clients> &client){_inviteClient = client;}
