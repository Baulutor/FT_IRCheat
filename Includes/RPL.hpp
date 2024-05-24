/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nibernar <nibernar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:57:44 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/24 11:21:08 by nibernar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
# define RPL_HPP

#define RPL_CMD_JOIN(Nickname, Username, Hostname, Channel, Key) (":" + Nickname + "!" + Username + "@" + Hostname + " JOIN :" + Channel + " " + Key + "\n\r")
#define RPL_CMD_PONG(Nickname, Username, Hostname, Key) (":" + Nickname + "!" + Username + "@" + Hostname + " PONG " + Key + "\n\r")
#define RPL_CMD_KICK(Nickname, Username, Hostname, Channel, target, com) (":" + Nickname + "!" + Username + "@" + Hostname + " KICK " + Channel + " " + target + " " + com + "\n\r")
#define RPL_CMD_TOPIC(Nickname, Channel, Topic) (":server 332 " + Nickname + " " + Channel + " :" + Topic + "\n\r")
#define RPL_CMD_NAME_LST_START(Nickname, Channel, user) (":server 353 " + Nickname + " = " + Channel + " :" + user + "\n\r")
#define RPL_CMD_NAME_LST_END(Nickname, channel) (":server 366 " + Nickname + " " + channel + " :End of /NAMES list" + "\n\r")
#define ERR_NEEDMOREPARAMS(Nickname, cmd) (":server 461 " + Nickname + " " + cmd + " :Not enough parameters\r\n")
#define ERR_NOSUCHCHANNEL(client, channel) (":server 403 " + client + " " + channel + " :No such channel\r\n")
#define ERR_USERNOTFOUND(client, Nickname, channel) (":server 441 " + client + " " + Nickname + " " + channel + " :They aren't on that channel\r\n")
#define RPL_WELCOME(Nickname, Username, Hostname) (":server 001 " + Nickname + " :Welcome to the Internet Relay Network " + Username + "@" + Hostname + "\n\r")
#define RPL_MOTD_START(Nickname) (":server 375 " + Nickname + " :- Message of the day -" + "\n\r")
#define RPL_MOTD_MSG(Nickname, MSG) (":server 372 " + Nickname + " :" + MSG + "\n\r")
#define RPL_MOTD_END(Nickname) (":server 376 " + Nickname + " :End of /MOTD command" + "\n\r")
#define RPL_CMD_PRIVMSG(Nickname, Username, Hostname, Channel, Message) (":" + Nickname + "!" + Username + "@" + Hostname + " PRIVMSG " + Channel + " :" + Message + "\n\r")
#define RPL_JOIN_ERROR_NAME(Nickname, Channel) (":server 403 " + Nickname + " :The channel (" + Channel + ") name must start with a #. Example : #" + Channel  + "\n\r")
#define RPL_JOIN_NOTICE(Nickname, Channel) (":server NOTICE " + Channel + " :" + Nickname + " Just join the channel" + "\n\r")
#endif

