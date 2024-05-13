/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:13:36 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/13 10:28:20 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"

void Join(std::string cmd, Clients client)
{
    std::cout << "JOIN" << std::endl;
    std::string reponse = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getAddrIp() + " JOIN :" + cmd.substr(6, cmd.find(' ', 6) - 6);
    if (send(client.getFd(), reponse.c_str(), reponse.size(), 0) == -1)
        throw (SendErrorExeption());
}