/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:27:47 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/07 11:48:57 by bfaure           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTS_HPP
# define CLIENTS_HPP

# include "FT_IRCheat.hpp"

class Clients
{
    public:
        Clients();
        ~Clients();
        
        void setFd(int fd);
        void setAddrIp(std::string addrIp);
        void setNickname(std::string nickname);
        void setUsername(std::string username);
        void setPass(std::string pass);
        int getFd() const;

        std::string getAddrIp() const;
        std::string getNickname() const;
        std::string getUsername() const;
        std::string getPass() const;
        std::vector<std::string> getChannels() const;
        std::vector<std::string> getChannelsInvite() const;

        void addChannel(std::string channel);
        void addChannelInvite(std::string channel);

        void printInfo();

    private:
        std::string _nickname;
        std::string _username;
        std::string _pass;
        int _fd;
        std::string _addrIp;
        std::vector<std::string> _channels;
        std::vector<std::string> _channelsInvite;
};

#endif