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

Channels::Channels() {}

Channels::Channels(std::string name, Clients& op)
{
    _name = name;
    _operator = &op;
    _topic = "No topic set";
}

Channels::~Channels() {}

// Getter

std::string Channels::getName() const {return (_name);}

std::string Channels::getTopic() const {return (_topic);}

std::map<std::string, Clients> Channels::getClientMap() {return (_clients);}

Clients Channels::getOperator() const {return (*_operator);}

// Setter

void Channels::setName(std::string name) {_name = name;}

void Channels::setTopic(std::string topic) {_topic = topic;}

void Channels::setOperator(Clients& op) {_operator = &op;}
>>>>>>> origin/ben
