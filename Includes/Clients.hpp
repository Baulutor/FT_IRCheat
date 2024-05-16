/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:27:47 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/14 11:17:37 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTS_HPP
# define CLIENTS_HPP

# include "FT_IRCheat.hpp"
# include "Channels.hpp"

class Channels;

class Clients
{
    public:
        Clients();
        //~Clients();
        
        void setFd(int fd);
        void setAddrIp(std::string addrIp);
        void setNickname(std::string nickname);
        void setUsername(std::string username);
        void setPass(std::string pass);

        bool initClients(std::string line);

        int getFd() const;
        std::string getAddrIp() const;
        std::string getNickname() const;
        std::string getUsername() const;
        std::string getPass() const;
        std::map<std::string, Channels>& getChannels();
        std::map<std::string, Channels> getChannelsInvite();

        void addChannel(std::string channel);
        void addChannelInvite(std::string channel);

        // Some shit to delete before production
        void printInfo();
        void printChannels();

    private:
        std::string _nickname;
        std::string _username;
        std::string _pass;
        int _fd;
        std::string _addrIp;
        std::map<std::string, Channels> _channels;
        std::map<std::string, Channels> _channelsInvite;
};

#endif