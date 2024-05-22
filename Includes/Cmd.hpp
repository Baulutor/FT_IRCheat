/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:14:18 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/22 13:48:14 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_HPP
# define CMD_HPP

# include "Clients.hpp"
# include "Channels.hpp"
# include "Server.hpp"
// # include "FT_IRCheat.hpp"

class Server;

class Clients;

class Channel;

void	Join(std::string cmd, Clients& client, Server& server);
void	Kick(std::string cmd, Clients& client, Server& server);
void	Privmsg(std::string cmd, Clients& client, Server& server);
void	Pong(std::string cmd, Clients& client, Server& server);
void	Invite(std::string cmd, Clients& client, Server& server);

class SendErrorExeption : public std::exception
{
	public :
		virtual ~SendErrorExeption() throw() {}
		virtual const char* what() const throw() {return ("Error while sending command to Client");}
};

#endif

