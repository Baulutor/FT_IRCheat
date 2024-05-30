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
# include <map>
#include <ctime>

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
		std::string getTimeTopic()const;
        std::map<std::string, Clients>& getClientMap();
		std::map<std::string, Clients>& getClientInvited();

//        Clients getOperator() const;
        std::string getPassword() const;
        std::vector<Clients> getOperator() const;
        std::string getMode(std::string target) const;
        int getLimit() const;
        int getNbClients() const;
        Clients getOperator(std::string target) const;

        // Setter
        void setName(std::string name);
        void setTopic(std::string topic);
		void setTimeTopic(std::string timeTopic);
        void setOperator(Clients& op);
        void setPassword(std::string password);
        void setMode(std::string target, std::string mode);
        void setClients(std::map<std::string, Clients> clients);
        void setLimit(int limit);
        
        void incrementNbClients();

        void removeMode(std::string target, std::string mode);
        void removeOperator(Clients& op);
	void ClientInvite(std::map<std::string, Clients> &clients);

        //Invite
        void    checkClientMapChannel();
	bool	checkIfOpeUserForInvite(Clients client);

	void convertTimestampToDateString();

    private :
        std::string _name;
        std::string _topic;
		std::string _timeTopic;
        std::string _password;
        // std::string _mode;
        std::map<std::string, std::string> _mode;
        // bool _isPrivate;
        int _nbClients;
        int _nbMaxClients;
        std::map<std::string, Clients> _clients;
        std::vector<Clients> _operator;
		std::map<std::string, Clients>	_inviteClient;

};

#endif
