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
#include <map>

Channels::Channels() {}

Channels::Channels(std::string name, Clients& op)
{
    _name = name;
    _operator.push_back(op);
    _topic = "No topic set";
}

// Channels::Channels(std::string name, Clients& op)
// {
//     _name = name;
//     _operator.push_back(op);
//     _topic = "No topic set";
//     _password = "";
//     _mode.insert(std::make_pair(name, " "));
//     _mode.insert(std::make_pair(op.getNickname(), "o"));
// }

Channels::~Channels() {}

// Getter

std::string Channels::getName() const {return (_name);}

std::string Channels::getTopic() const {return (_topic);}

std::map<std::string, Clients>& Channels::getClientMap() {return (_clients);}

std::map<std::string, Clients>& Channels::getClientInvited() {return (_inviteClient);}

//Clients Channels::getOperator() const {return (*_operator);}


std::string Channels::getPassword() const {return (_password);}

std::string Channels::getMode(std::string target) const {return (_mode.find(target)->second);}

//Clients Channels::getOperator() const {return (*_operator);}

std::vector<Clients> Channels::getOperator() const {return (_operator);}

// Setter

void Channels::setName(std::string name) {_name = name;}

void Channels::setTopic(std::string topic) {_topic = topic;}

//void Channels::setOperator(Clients& op) {_operator = &op;}

void Channels::setOperator(Clients& op) {
    _operator.push_back(op);
}

void Channels::setPassword(std::string password) {_password = password;}

void Channels::setMode(std::string target, std::string mode)
{
    std::map<std::string, std::string>::iterator it = _mode.find(target);
    if (it != _mode.end())
        it->second += mode;
    else
        _mode.insert(std::make_pair(target, mode));
}

void Channels::setClients(std::map<std::string, Clients> clients) {_clients = clients;}

void Channels::ClientInvite(std::map<std::string, Clients> &client){_inviteClient = client;}
