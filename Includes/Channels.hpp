/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channels.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 11:51:13 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/07 11:51:13 by bfaure           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNELS_HPP
# define CHANNELS_HPP

# include "FT_IRCheat.hpp"
# include "Clients.hpp"

class Clients;

class Channels
{
    public :
        // Constructors / Destructor
        Channels();
        Channels(std::string name, Clients& op);
        ~Channels();

        // Getter
        std::string getName() const;
        std::string getTopic() const;
        std::map<std::string, Clients>& getClientMap();
        Clients getOperator() const;

        // Setter
        void setName(std::string name);
        void setTopic(std::string topic);
        void setOperator(Clients& op);
        void setClients(std::map<std::string, Clients> clients);

    private :
        std::string _name;
        std::string _topic;
        std::string _password;
        std::string _mode;
        // bool _isPrivate;
        // int _nbClients;
        // int _nbMaxClients;
        std::map<std::string, Clients> _clients;
        Clients *_operator;
};

#endif
