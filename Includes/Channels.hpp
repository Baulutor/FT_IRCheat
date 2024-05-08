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
# include <map>
# include <vector>
 
class Channel
{
    public :
        Channel();
        Channel(std::string name, Client operator);
        ~Channel();

    private :
        std::string _name;
        std::string _topic;
        std::string _password;
        bool _isPrivate;
        int _nbClients;
        int _nbMaxClients;
        std::map<int, Client> _clients;
        Client *_operator;
};

#endif
