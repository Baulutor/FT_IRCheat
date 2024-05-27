/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:42:28 by bfaure            #+#    #+#             */
/*   Updated: 2024/05/27 20:55:21 by bfaure           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Cmd.hpp"

bool isChannel(std::string target)
{
    if (target.find('#') != std::string::npos)
        return (true);
    return (false);
}

bool isMode(std::string target)
{
    std::cout << "target isMode = |" << target << "|" << std::endl;
    if (target.find_first_of("it") != std::string::npos)
        return (true);
    if (target.find("-") != std::string::npos && target.find("l") != std::string::npos)
        return (true);
    if (target.find("-") != std::string::npos && target.find("k") != std::string::npos)
        return (true);
    return (false);
}

bool isArgsMode(std::string target)
{
    std::cout << "target isArgsMode = |" << target << "|" << std::endl;
    if (target.find_first_of("o") != std::string::npos)
        return (true);
    if (target.find("+") != std::string::npos && target.find("l") != std::string::npos)
        return (true);
    if (target.find("+") != std::string::npos && target.find("k") != std::string::npos)
        return (true);
    return (false);
}

bool checkChannel(std::string target, Server& server, Clients& client, std::map<std::string, Channels>::iterator channelIt)
{
    if (channelIt == server.getChannels().end())
        return (sendCmd(ERR_NOSUCHCHANNEL(client.getNickname(), target), client), false);
    return (true);
}

char getMode(std::string target)
{
    static int i = 0;
    if (target[i] == '+' || target[i] == '-')
        i++;
    return (target[i]);
}

bool addOpMode(std::vector<std::string> args, size_t i, std::map<std::string, Channels>::iterator channelIt)
{
    std::cout << "getMode(args[i]) = |" << getMode(args[i]) << "|" << std::endl;
    std::map<std::string, Clients>::iterator clientIt = channelIt->second.getClientMap().find(args[i]);
    if (clientIt != channelIt->second.getClientMap().end())
    {
        std::cout << "channelIt->second.getMode(args[i]) = |" << channelIt->second.getMode(args[i]) << "|" << std::endl;
        if (channelIt->second.getMode(args[i]).find('o') == std::string::npos)
        {
            channelIt->second.setMode(args[i], "o");
            channelIt->second.setOperator(clientIt->second);
            NameLstUpadte(clientIt->second, channelIt->second);
            return (true);
        }
    }
    return (false);
}

bool removeOpMode(std::vector<std::string> args, size_t i, std::map<std::string, Channels>::iterator channelIt)
{
    std::map<std::string, Clients>::iterator clientIt = channelIt->second.getClientMap().find(args[i]);
    if (clientIt != channelIt->second.getClientMap().end())
    {
        std::cout << "channelIt->second.getMode(args[i]) = |" << channelIt->second.getMode(args[i]) << "|" << std::endl;
        if (channelIt->second.getMode(args[i]).find('o') != std::string::npos)
        {
            channelIt->second.removeMode(args[i], "o");
            channelIt->second.removeOperator(clientIt->second);
            NameLstUpadte(clientIt->second, channelIt->second);
            return (true);
        }
    }
    return (false);
}

bool addInviteMode(std::string target, std::map<std::string, Channels>::iterator channelIt)
{
    std::cout << "channelIt->second.getMode(target) = |" << channelIt->second.getMode(target) << "|" << std::endl;
    if (channelIt->second.getMode(target).find('i') == std::string::npos)
    {
        channelIt->second.setMode(target, "i");
        return (true);
    }
    return (false);
}

bool removeInviteMode(std::string target, std::map<std::string, Channels>::iterator channelIt)
{
    std::cout << "channelIt->second.getMode(target) = |" << channelIt->second.getMode(target) << "|" << std::endl;
    if (channelIt->second.getMode(target).find('i') != std::string::npos)
    {
        channelIt->second.removeMode(target, "i");
        return (true);
    }
    return (false);
}

bool addTopicMode(std::string target, std::map<std::string, Channels>::iterator channelIt)
{
    std::cout << "channelIt->second.getMode(target) = |" << channelIt->second.getMode(target) << "|" << std::endl;
    if (channelIt->second.getMode(target).find('t') != std::string::npos)
    {
        channelIt->second.setMode(target, "t");
        return (true);
    }
    return (false);
}

bool removeTopicMode(std::vector<std::string> args, size_t i, std::map<std::string, Channels>::iterator channelIt)
{
    std::cout << "channelIt->second.getTopic() = |" << channelIt->second.getTopic() << "|" << std::endl;
    if (channelIt->second.getMode(args[i]).find('t') != std::string::npos)
    {
        channelIt->second.removeMode(args[i], "t");
        return (true);
    }
    return (false);
}

bool addKeyMode(std::string target, std::vector<std::string> args, size_t i, std::map<std::string, Channels>::iterator channelIt)
{
    std::cout << "channelIt->second.getMode(target) = |" << channelIt->second.getMode(target) << "|" << std::endl;
    if (channelIt->second.getMode(target).find('k') == std::string::npos)
    {
        channelIt->second.setMode(target, "k");
        channelIt->second.setPassword(args[i]);
        return (true);
    }
    return (false);
}

bool removeKeyMode(std::string target, std::map<std::string, Channels>::iterator channelIt)
{
    std::cout << "channelIt->second.getMode(target) = |" << channelIt->second.getMode(target) << "|" << std::endl;
    if (channelIt->second.getMode(target).find('k') != std::string::npos)
    {
        channelIt->second.removeMode(target, "k");
        return (true);
    }
    return (false);
}

bool addLimitMode(std::string target, std::vector<std::string> args, size_t i, std::map<std::string, Channels>::iterator channelIt)
{
    std::cout << "channelIt->second.getMode(target) = |" << channelIt->second.getMode(target) << "|" << std::endl;
    if (channelIt->second.getMode(target).find('l') == std::string::npos)
    {
        channelIt->second.setMode(target, "l");
        channelIt->second.setLimit(atoi(args[i].c_str()));
        return (true);
    }
    return (false);
}

bool removeLimitMode(std::string target, std::map<std::string, Channels>::iterator channelIt)
{
    std::cout << "channelIt->second.getMode(target) = |" << channelIt->second.getMode(target) << "|" << std::endl;
    if (channelIt->second.getMode(target).find('l') != std::string::npos)
    {
        channelIt->second.removeMode(target, "l");
        return (true);
    }
    return (false);
}

void checkArgs(std::vector<std::string> args, std::string modes, Clients& client, std::map<std::string, Channels>::iterator channelIt)
{
    size_t i = 0;
    (void)client;
    std::cout << "checkArgs modes = |" << modes << "|" << std::endl;
    while (modes[i])
    {
        std::cout << "modes[i] = |" << modes[i] << "|" << std::endl;
        if (modes[i] == '+')
        {
            if (getMode(modes) == 'o')
                addOpMode(args, i, channelIt);
            else if (getMode(modes) == 'i')
                addInviteMode(channelIt->first, channelIt);
            else if (getMode(modes) == 't')
                addTopicMode(channelIt->first, channelIt);
        }
        if (modes[i] == '-')
        {
            if (getMode(modes) == 'o')
                removeOpMode(args, i, channelIt);
            else if (getMode(modes) == 'i')
                removeInviteMode(channelIt->first, channelIt);
            else if (getMode(modes) == 't')
                removeTopicMode(args, i, channelIt);
        }
        i++;
    }
    // return (false);
}

void Mode(std::string cmd, Clients& client, Server& server)
{
    std::cout << "Mode" << std::endl;
    cmd.erase(0, 5);
    std::vector<std::string> tokens = split(cmd, ' ');
    std::map<std::string, Channels>::iterator channelIt = server.getChannels().find(tokens[0]);
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        std::cout << "tokens: |" << *it << "|" << std::endl;
    }
    if (tokens.size() < 1)
        return (sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), tokens[0]), client));
    else if (tokens.size() == 1)
    {
        if (!checkChannel(tokens[0], server, client, channelIt))
            return ;
        return (sendCmd(RPL_CHANNELMODEIS(client.getNickname(), tokens[0], channelIt->second.getMode(tokens[0])), client));
    }
    else if (tokens.size() == 2)
    {
        if (!checkChannel(tokens[0], server, client, channelIt))
            return ;
        if (channelIt->second.getClientMap().find(tokens[1]) == channelIt->second.getClientMap().end())
        {
            if (!isMode(tokens[1]) && (tokens[1][0] != '-' || tokens[1][0] != '+'))
            {
                if (isArgsMode(tokens[1]) && (tokens[1][0] != '-' || tokens[1][0] != '+'))
                    return (sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), tokens[0]), client));
                else
                    return (sendCmd(ERR_UMODEUNKNOWNFLAG(client.getNickname(), tokens[1]), client));
            }
        }
        else
            return (sendCmd(RPL_UMODEIS(client.getNickname(), tokens[1], channelIt->second.getMode(tokens[1])), client));
    }
    else if (tokens.size() >= 3)
    {
        if (!checkChannel(tokens[0], server, client, channelIt))
            return ;
        if (isMode(tokens[1]))
            return (sendCmd(ERR_INVALIDMODEPARAM(client.getNickname(), tokens[1], tokens[2]), client));
        if (!isArgsMode(tokens[1]) && (tokens[1][0] != '-' || tokens[1][0] != '+'))
            return (sendCmd(ERR_UMODEUNKNOWNFLAG(client.getNickname(), tokens[1]), client)); 
        std::vector<std::string> args(tokens.begin() + 2, tokens.end());
        checkArgs(args, tokens[1], client, channelIt);
    }
}

