/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clients.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:27:47 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/06 16:00:21 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENTS_HPP
# define CLIENTS_HPP

# include <string>
# include <cstdlib>
# include <iostream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>

class Clients
{
    public:
        Clients();
        ~Clients();
        void setFd(int fd);
        void setAddrIp(std::string addrIp);
        void setNick(std::string nick);
        void setPass(std::string pass);
        int getFd() const;
        std::string getAddrIp() const;
        std::string getNick() const;
        std::string getPass() const;
        void printInfo();
    private:
        std::string _nick;
        std::string _pass;
        int _fd;
        std::string _addrIp;
};

#endif