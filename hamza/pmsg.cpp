/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pmsg.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamrachi <hamrachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 22:18:01 by hamrachi          #+#    #+#             */
/*   Updated: 2025/07/17 00:56:12 by hamrachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

std::vector<std::string> ft_split(const std::string &line)
{
    std::vector<std::string> result;
    std::stringstream ss(line);
    std::string word;
    while(ss >> word)
        result.push_back(word);
    return (result);
}

void Server::privateMessage(Client *client, std::string line)
{
    std::vector<std::string> args = ft_split(line);

    if (args.size() < 3)
    {
        sendReply(client->getSockfd(), "ERR_NEEDMOREPARAMS :Not enough parameters\r\n");
        return;
    }

    std::string target = args[1];
    std::string message;

    if (args[2][0] != ':')
    {
        sendReply(client->getSockfd(), "ERR_NOTEXTTOSEND :Message must start with ':'\r\n");
        return;
    }
    
    message = args[2].substr(1);
    for (size_t i = 3; i < args.size(); i++)
        message += " " + args[i];

    if (target.empty() || message.empty())
    {
        sendReply(client->getSockfd(), "ERR_NOTEXTTOSEND :No text to send\r\n");
        return;
    }

    Client *receiver = findClientByNickname(target);
    if (!receiver)
    {
        sendReply(client->getSockfd(), "ERR_NOSUCHNICK :No such nick\r\n");
        return;
    }

    std::string finalMsg = ":" + client->getNickname() + "!" + client->getUsername() + "@localhost PRIVMSG " + target + " :" + message + "\r\n";
    sendReply(receiver->getSockfd(), finalMsg);
}
