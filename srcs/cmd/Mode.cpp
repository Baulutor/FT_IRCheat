/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfaure <bfaure@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 11:42:28 by bfaure            #+#    #+#             */
/*   Updated: 2024/06/04 14:49:36 by bfaure           ###   ########.fr       */
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

char getMode(std::string target, size_t i)
{
    // static int i = 0;
    if (target[i] == '+' || target[i] == '-')
        i++;
    return (target[i]);
}

int addOpMode(std::vector<std::string> args, size_t i, std::map<std::string, Channels>::iterator channelIt, Server& server)
{
    // int fdClient = findFdClientByName(args[i], server.getClients());
    while (i >= args.size())
        i--;
    int fdClient = server.getFdClientByName(args[i]);
    std::map<int, Clients>::iterator clientIt = channelIt->second.getClientMap().find(fdClient);
    if (clientIt != channelIt->second.getClientMap().end())
    {
        std::cout << "channelIt->second.getMode(args[i]) = |" << channelIt->second.getMode(fdClient) << "|" << std::endl;
        if (server.getChannels().find(channelIt->first)->second.getMode(fdClient).find('o') == std::string::npos)
        {
            server.getChannels().find(channelIt->first)->second.setMode(fdClient, "o");
            server.getChannels().find(channelIt->first)->second.setOperator(clientIt->second);
            NameLstUpdate(clientIt->second, channelIt->second);
            return (1);
        }
    }
    return (1);
}

int removeOpMode(std::vector<std::string> args, size_t i, std::map<std::string, Channels>::iterator channelIt, Server& server)
{
    // int fdClient = findFdClientByName(args[i], server.getClients());
    while (i >= args.size())
        i--;
    int fdClient = server.getFdClientByName(args[i]);
    std::map<int, Clients>::iterator clientIt = channelIt->second.getClientMap().find(fdClient);
    if (clientIt != channelIt->second.getClientMap().end())
    {
        std::cout << "channelIt->second.getMode(fdClient) = |" << channelIt->second.getMode(fdClient) << "|" << std::endl;
        if (server.getChannels().find(channelIt->first)->second.getMode(fdClient).find('o') != std::string::npos)
        {
            server.getChannels().find(channelIt->first)->second.removeMode(fdClient, "o");
            server.getChannels().find(channelIt->first)->second.removeOperator(clientIt->second);
            NameLstUpdate(clientIt->second, channelIt->second);
            return (1);
        }
    }
    return (1);
}

int addInviteMode(int target, std::map<std::string, Channels>::iterator channelIt)
{
    std::cout << "target addInviteMode = |" << target << "|" << std::endl;
    std::cout << "channelIt->second.getMode(target) = |" << channelIt->second.getMode(target) << "|" << std::endl;
    std::cout << "channelIt->second.getMode(target).find('i') = |" << channelIt->second.getMode(target).find('i') << "|" << std::endl;
    if (channelIt->second.getMode(target).find('i') == std::string::npos)
    {
        channelIt->second.setMode(target, "i");
        return (1);
    }
    return (1);
}

int removeInviteMode(int target, std::map<std::string, Channels>::iterator channelIt)
{
    std::cout << "target = |" << target << "|" << std::endl;
    std::cout << "channelIt->second.getMode(target) = |" << channelIt->second.getMode(target) << "|" << std::endl;
    if (channelIt->second.getMode(target).find('i') != std::string::npos)
    {
        channelIt->second.removeMode(target, "i");
        return (1);
    }
    return (1);
}

int addTopicMode(int target, std::map<std::string, Channels>::iterator channelIt)
{
    if (channelIt->second.getMode(target).find('t') == std::string::npos)
    {
        channelIt->second.setMode(target, "t");
        return (1);
    }
    return (1);
}

int removeTopicMode(int target, std::map<std::string, Channels>::iterator channelIt)
{
    std::cout << "channelIt->second.getTopic() = |" << channelIt->second.getTopic() << "|" << std::endl;
    if (channelIt->second.getMode(target).find('t') != std::string::npos)
    {
        channelIt->second.removeMode(target, "t");
        return (1);
    }
    return (1);
}

int addKeyMode(int target, std::vector<std::string> args, size_t i, std::map<std::string, Channels>::iterator channelIt)
{
    std::cout << "target = |" << target << "|" << std::endl;
    while (i >= args.size())
        i--;
    std::cout << "channelIt->second.getMode(target) = |" << channelIt->second.getMode(target) << "|" << std::endl;
    if (channelIt->second.getMode(target).find('k') == std::string::npos)
    {
        channelIt->second.setMode(target, "k");
        channelIt->second.setPassword(args[i]);
        return (1);
    }
    return (1);
}

int removeKeyMode(int target, std::map<std::string, Channels>::iterator channelIt)
{
    std::cout << "target = |" << target << "|" << std::endl;
    std::cout << "channelIt->second.getMode(target) = |" << channelIt->second.getMode(target) << "|" << std::endl;
    if (channelIt->second.getMode(target).find('k') != std::string::npos)
    {
        channelIt->second.removeMode(target, "k");
        return (1);
    }
    return (1);
}

int addLimitMode(int target, std::vector<std::string> args, size_t i, std::map<std::string, Channels>::iterator channelIt)
{
    std::cout << "target = |" << target << "|" << std::endl;
    while (i >= args.size())
        i--;
    std::cout << "channelIt->second.getMode(target) = |" << channelIt->second.getMode(target) << "|" << std::endl;
    if (channelIt->second.getMode(target).find('l') == std::string::npos)
    {
		if (args.size() == 0)
			return 2;
        channelIt->second.setMode(target, "l");
		channelIt->second.setLimit(atoi(args[i].c_str()));
        return (1);
    }
    return (1);
}

int removeLimitMode(int target, std::map<std::string, Channels>::iterator channelIt)
{
    std::cout << "target = |" << target << "|" << std::endl;
    std::cout << "channelIt->second.getMode(target) = |" << channelIt->second.getMode(target) << "|" << std::endl;
    if (channelIt->second.getMode(target).find('l') != std::string::npos)
    {
        channelIt->second.removeMode(target, "l");
        return (1);
    }
    return (1);
}

void checkArgs(std::vector<std::string> args, std::string modes, Clients& client, std::map<std::string, Channels>::iterator channelIt, Server& server)
{
    size_t i = 0;
    bool plusSign = false;
    bool minusSign = false;
    (void)client;
    std::cout << "checkArgs modes = |" << modes << "|" << std::endl;
    while (i < modes.size())
    {
        std::cout << "modes[i] = |" << modes[i] << "|" << std::endl;
        if (modes[i] == '+' || plusSign == true)
        {
            std::cout << "plusSign = " << (plusSign ? "true" : "false") << std::endl;
            minusSign = false;
            plusSign = true;
            std::cout << "channelIt->first = |" << channelIt->first << "|" << std::endl;
            if (getMode(modes, i) == 'i')
                i += addInviteMode(server.getFdClientByName(channelIt->first), channelIt);
            if (getMode(modes, i) == 't')
                i += addTopicMode(server.getFdClientByName(channelIt->first), channelIt);
			if (args.size() == 0 && isArgsMode(modes))
			{
				sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"), client);
				return ;
			}
            if (getMode(modes, i) == 'o')
                i += addOpMode(args, i, channelIt, server);
            if (getMode(modes, i) == 'k')
                i += addKeyMode(server.getFdClientByName(channelIt->first), args, i, channelIt);
            if (getMode(modes, i) == 'l')
                i += addLimitMode(server.getFdClientByName(channelIt->first), args, i, channelIt);
        }
        if (modes[i] == '-' || minusSign == true)
        {
            std::cout << "minusSign = " << (minusSign ? "true" : "false") << std::endl;
            plusSign = false;
            minusSign = true;
            if (getMode(modes, i) == 'o')
                i += removeOpMode(args, i, channelIt, server);
            if (getMode(modes, i) == 'i')
                i += removeInviteMode(server.getFdClientByName(channelIt->first), channelIt);
            if (getMode(modes, i) == 't')
                i += removeTopicMode(server.getFdClientByName(channelIt->first), channelIt);
            if (getMode(modes, i) == 'k')
                i += removeKeyMode(server.getFdClientByName(channelIt->first), channelIt);
            if (getMode(modes, i) == 'l')
                i += removeLimitMode(server.getFdClientByName(channelIt->first), channelIt);
        }
        i++;
    }
    // return (false);
}

void Mode(std::string cmd, Clients& client, Server& server)
{
    std::cout << "Mode" << std::endl;
    std::cout << "cmd = |" << cmd << "|" << std::endl;
    cmd.erase(0, 5);
    std::cout << "cmd erase = |" << cmd << "|" << std::endl;
    std::vector<std::string> tokens = splitInit(cmd, ' ');
    std::cout << "tokens.size() = " << tokens.size() << std::endl;
    if (tokens.size() == 0)
        return (sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"), client));
    std::map<std::string, Channels>::iterator channelIt = server.getChannels().find(tokens[0]);
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
        if (*it == "WHO")
        {
            tokens.erase(it, tokens.end());
            break;
        }
        std::cout << "tokens: |" << *it << "|" << std::endl;
    }
    if (channelIt == server.getChannels().end())
        return (sendCmd(ERR_NOSUCHCHANNEL(client.getNickname(), tokens[0]), client));
    if (channelIt->second.getOperator(client.getFd()).getFd() != client.getFd())
    {
        std::cout << "ERR_CHANOPRIVSNEEDEDED = |" << ERR_CHANOPRIVSNEEDED(client.getNickname(), tokens[0]) << "|" << std::endl;
        return (sendCmd(ERR_CHANOPRIVSNEEDED(client.getNickname(), tokens[0]), client));
    }
    if (tokens.size() < 1)
        return (sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"), client));
    else if (tokens.size() == 1)
    {
        if (!checkChannel(tokens[0], server, client, channelIt))
            return ;
        return (sendCmd(RPL_CHANNELMODEIS(client.getNickname(), tokens[0], channelIt->second.getMode(-42)), client));
    }
    else if (tokens.size() == 2)
    {
        if (!checkChannel(tokens[0], server, client, channelIt))
            return ;
        int fdClient = findFdClientByName(tokens[1], server.getClients());
        std::map<int, Clients>::iterator clientIt = channelIt->second.getClientMap().find(fdClient);
        if (clientIt == channelIt->second.getClientMap().end())
        {
            if (!isMode(tokens[1]) && (tokens[1][0] != '-' || tokens[1][0] != '+'))
            {
                if (isArgsMode(tokens[1]) && (tokens[1][0] != '-' || tokens[1][0] != '+'))
                    return (sendCmd(ERR_NEEDMOREPARAMS(client.getNickname(), "MODE"), client));
                else
                    return (sendCmd(ERR_UMODEUNKNOWNFLAG(client.getNickname(), tokens[1]), client));
            }
            else
            {
                std::vector<std::string> args(tokens.begin() + 2, tokens.end());
                checkArgs(args, tokens[1], client, channelIt, server);
            }
        }
        else
            return (sendCmd(RPL_UMODEIS(client.getNickname(), tokens[1], channelIt->second.getMode(server.getFdClientByName(tokens[1]))), client));
    }
    else if (tokens.size() >= 3)
    {
        if (!checkChannel(tokens[0], server, client, channelIt))
            return ;
        if ((isMode(tokens[1]) && !isArgsMode(tokens[1])) && (tokens[1][0] != '-' || tokens[1][0] != '+'))
            return (sendCmd(ERR_INVALIDMODEPARAM(client.getNickname(), tokens[1], tokens[2]), client));
        // if (!isArgsMode(tokens[1]) && (tokens[1][0] != '-' || tokens[1][0] != '+'))
        //     return (sendCmd(ERR_UMODEUNKNOWNFLAG(client.getNickname(), tokens[1]), client)); 
        std::vector<std::string> args(tokens.begin() + 2, tokens.end());
        checkArgs(args, tokens[1], client, channelIt, server);
    }
}

