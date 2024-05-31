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
    _mode.insert(std::make_pair(name, "t"));
    _mode.insert(std::make_pair(op.getNickname(), "o"));
}

Channels::~Channels() {}

// Getter

std::string Channels::getName() const {return (_name);}

std::string Channels::getTopic() const {return (_topic);}

std::string Channels::getTimeTopic() const {return (_timeTopic);}

std::map<std::string, Clients>& Channels::getClientMap() {return (_clients);}

std::map<std::string, Clients>& Channels::getClientInvited() {return (_inviteClient);}

std::string Channels::getPassword() const {return (_password);}

std::string Channels::getMode(std::string target) const {return (_mode.find(target)->second);}

int Channels::getLimit() const {return (_nbMaxClients);}

int Channels::getNbClients() const {return (_nbClients);}

std::vector<Clients> Channels::getOperatorVector() const{return (_operator);}

Clients Channels::getOperator(std::string target) const
{
    for (size_t i = 0; i < _operator.size(); i++)
    {
        if (_operator[i].getNickname() == target)
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

void Channels::setMode(std::string target, std::string mode)
{
    std::cout << "setMode Mode = " << mode << std::endl;
    std::map<std::string, std::string>::iterator it = _mode.find(target);
    if (it != _mode.end())
        it->second += mode;
    else
        _mode.insert(std::make_pair(target, mode));
}

void Channels::setLimit(int limit) {_nbMaxClients = limit;}

void Channels::setClients(std::map<std::string, Clients> clients) {_clients = clients;}

void Channels::incrementNbClients() {_nbClients++;}

// Remover

void Channels::removeMode(std::string target, std::string mode)
{
    std::cout << "removeMode Mode = " << mode << std::endl;
    std::map<std::string, std::string>::iterator it = _mode.find(target);
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
        if (_operator[i].getNickname() == op.getNickname())
            _operator.erase(_operator.begin() + i);
    }
}
void Channels::ClientInvite(std::map<std::string, Clients> &client){_inviteClient = client;}
