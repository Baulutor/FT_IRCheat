/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:14:18 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/27 20:54:34 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_HPP
# define CMD_HPP

// # include "FT_IRCheat.hpp"
# include "Clients.hpp"
# include "Channels.hpp"
# include "Server.hpp"

class Channels;

void	Invite(std::string cmd, Clients& client, Server& server);
void 	Join(std::string cmd, Clients& client, Server& server);
void 	Kick(std::string cmd, Clients& client, Server& server);
void 	Privmsg(std::string cmd, Clients& client, Server& server);
void 	Pong(std::string cmd, Clients& client, Server& server);
void 	Mode(std::string cmd, Clients& client, Server& server);
void 	Topic(std::string cmd, Clients& client, Server& server);

class SendErrorExeption : public std::exception
{
	public :
		virtual ~SendErrorExeption() throw() {}
		virtual const char* what() const throw() {return ("Error while sending command to Client");}
};

#endif

