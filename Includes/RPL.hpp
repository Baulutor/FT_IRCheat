/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:57:44 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/16 14:10:22 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
# define RPL_HPP

#define RPL_CMD_JOIN(Nickname, Username, Hostname, Channel, Key) (":" + Nickname + "!" + Username + "@" + Hostname + " JOIN :" + Channel + " " + Key + "\n\r")
#define RPL_CMD_KICK(Nickname, Username, Hostname, Channel, target, com) (":" + Nickname + "!" + Username + "@" + Hostname + " KICK " + Channel + " " + target + " " + com + "\n\r")
#define RPL_CMD_TOPIC(Nickname, Channel, Topic) (":server 332 " + Nickname + " " + Channel + " :" + Topic + "\n\r")
#define RPL_CMD_NAME_LST_START(Nickname, Channel, user) (":server 353 " + Nickname + " = " + Channel + " :" + user + "\n\r")
#define RPL_CMD_NAME_LST_END(Nickname, channel) (":server 366 " + Nickname + " " + channel + " :End of /NAMES list" + "\n\r")
#define ERR_NEEDMOREPARAMS(Nickname, cmd) (":server 461 " + Nickname + " " + cmd + " :Not enough parameters\r\n")
#endif

