/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibernar <nibernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 17:15:55 by nibernar          #+#    #+#             */
/*   Updated: 2024/05/30 18:26:38 by nibernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"

/* 
    NICK <pseudo>
    regle a suivre pour le nickname
    - 9 char max
    - pas de string vide
    - pas de ',' ' ' '*' '?' '!' '@'
    - ne peu pas commencer par '#' '&'
    NICKLEN=9
*/

void 	Nick(std::string cmd, Clients& client, Server& server) {
    (void) client;
    (void) server;
    std::cout << "NICK = " << cmd << std::endl;
}