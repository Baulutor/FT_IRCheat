/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 16:57:44 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/22 13:48:01 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPL_HPP
# define RPL_HPP

//Invite
#define ERR_USERONCHANNEL(ClientNickname, nickname, Channel) (ClientNickname + " " + nickname + " " + Channel + " :is already on channel\n\r")
#define RPL_INVITING( nickname, channelName) (":server 341 " + nickname + " " + channelName + "\n\r")
#define RPL_INVITING_NOTICE(nickname, channelName) (":server NOTICE " + nickname + " " + channelName + "\n\r")
#define ERR_CHANOPRIVSNEEDED(nickname, channel) (":server 482 " + nickname + " " + channel + " :You\'re not channel operator\n\r")



# define RPL_CMD_JOIN(Nickname, Username, Hostname, Channel) (":" + Nickname + "!" + Username + "@" + Hostname + " JOIN :" + Channel + "\n\r")
# define RPL_CMD_JOIN_KEY(Nickname, Username, Hostname, Channel, Key) (":" + Nickname + "!" + Username + "@" + Hostname + " JOIN :" + Channel + " " + Key + "\n\r")
# define RPL_JOIN_ERROR_NAME(Nickname, Channel) (":server 403 " + Nickname + " :💩 The channel (" + Channel + ") name must start with a #. Example : #" + Channel  + "\n\r")
# define RPL_JOIN_NOTICE(Nickname, Channel) (":server NOTICE " + Channel + " :" + Nickname + " Just join the channel" + "\n\r")
# define RPL_JOIN_ERROR_KEY(Nickname, Channel) (":server 475 " + Nickname + " " + Channel + " :Cannot join channel (+k)" + "\n\r")

# define RPL_CMD_PONG(Nickname, Username, Hostname, Key) (":" + Nickname + "!" + Username + "@" + Hostname + " PONG " + Key + "\n\r")

# define RPL_CMD_KICK(Nickname, Username, Hostname, Channel, target, com) (":" + Nickname + "!" + Username + "@" + Hostname + " KICK " + Channel + " " + target + " " + com + "\n\r")

# define RPL_CMD_TOPIC(Nickname, Channel, Topic) (":server 332 " + Nickname + " " + Channel + " :" + Topic + "\n\r")

# define RPL_CMD_NAME_LST_START(Nickname, Channel, user) (":server 353 " + Nickname + " = " + Channel + " :" + user + "\n\r")
# define RPL_CMD_NAME_LST_END(Nickname, channel) (":server 366 " + Nickname + " " + channel + " :End of /NAMES list" + "\n\r")

# define ERR_NEEDMOREPARAMS(Nickname, cmd) (":server 461 " + Nickname + " " + cmd + " :Not enough parameters\r\n")

# define RPL_WELCOME(Nickname, Username, Hostname) (":server 001 " + Nickname + " :Welcome to the Internet Relay Network " + Username + "@" + Hostname + "\n\r")
# define RPL_MOTD_START(Nickname) (":server 375 " + Nickname + " :- Message of the day -" + "\n\r")
# define RPL_MOTD_MSG(Nickname, MSG) (":server 372 " + Nickname + " :" + MSG + "\n\r")
# define RPL_MOTD_END(Nickname) (":server 376 " + Nickname + " :End of /MOTD command" + "\n\r")

# define RPL_CMD_PRIVMSG(Nickname, Username, Hostname, Channel, Message) (":" + Nickname + "!" + Username + "@" + Hostname + " PRIVMSG " + Channel + " :" + Message + "\n\r")

# define RPL_ERROR_NICKNAME_IN_USE(Nickname, Nickname2) (":server 433 " + Nickname + " " + Nickname2 + " :Nickname is already in use" + "\n\r")
# define RPL_CMD_NICK(Nickname, Username, Hostname, NewNickname) (":" + Nickname + "!" + Username + "@" + Hostname + " NICK " + NewNickname + "\n\r")

# define RPL_CMD_MODE(Nickname, Username, Hostname, Channel, Mode, Target, Args) (":" + Nickname + "!" + Username + "@" + Hostname + " MODE " + Channel + " " + Mode + " " + Target + " " + Args + "\n\r")
# define ERR_NOSUCHNICK(Nickname, User) (":server 401 " + Nickname + " " + User + " :No such user" + "\n\r")
# define RPL_UMODEIS(Nickname, User, Mode) (":server 221 " + Nickname + " " + User + " :" + Mode + "\n\r")
# define ERR_UMODEUNKNOWNFLAG(Nickname, Mode) (":server 501 " + Nickname + " " + Mode + " :Unknown mode" + "\n\r")
# define ERR_NOSUCHCHANNEL(Nickname, Channel) (":server 403 " + Nickname + " " + Channel + " :No such channel" + "\n\r")
# define RPL_CHANNELMODEIS(Nickname, Channel, Mode) (":server 324 " + Nickname + " " + Channel + " " + Mode + "\n\r")

# define ERR_NOTONCHANNEL(Nickname, Channel) (":server 442 " + Nickname + " " + Channel + " :You are not on that channel" + "\n\r")

#define ERR_NOTCHANOP(Nickname, channel) (":server 482 " + Nickname + " " + channel + " :You're not channel operator\r\n")
#define ERR_USERNOTFOUND(client, Nickname, channel) (":server 441 " + client + " " + Nickname + " " + channel + " :They aren't on that channel\r\n")
#define ERR_PASSWDMISMATCH(client) (":server 441 " + client + " :Password incorrect \n\r")
#endif

