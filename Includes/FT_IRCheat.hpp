/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   FT_IRCheat.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:47:35 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/07 11:47:35 by bfaure           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRCHEAT_HPP
# define FT_IRCHEAT_HPP

# include <string>
# include <cstdlib>
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <vector>
# include <sstream>
# include "RPL.hpp"
# include "Clients.hpp"
// # include <map>

class Clients;

class Channels;

class Server;

std::vector<std::string> split(const std::string &s, char delim);
void sendCmd(const std::string& cmd, Clients &client);
void sendBrodcastChannel(const std::string& cmd, Channels& channel);
void sendBrodcastMSG(const std::string& cmd, Channels& channel, Clients& client);
void sendBrodcastServer(const std::string& cmd, Server& server);

#endif
