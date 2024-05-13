/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cmd.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/13 10:14:18 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/13 16:37:36 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CMD_HPP
# define CMD_HPP

# include "FT_IRCheat.hpp"
# include "Clients.hpp"
# include "Channels.hpp"

class Channel;

void Join(std::string cmd, Clients& client, std::map<std::string, Channels>& channelsServer);

class SendErrorExeption : public std::exception
{
	public :
		virtual ~SendErrorExeption() throw() {}
		virtual const char* what() const throw() {return ("Error while sending command to Client");}
};

#endif

