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
    this->_name = name;
    this->_operator = &op;
}

Channels::~Channels() {}

std::string Channels::getName() const
{
    return (this->_name);
}

std::map<int, Clients> Channels::getClientMap() {return (_clients);}
