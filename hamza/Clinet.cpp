/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Clinet.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamrachi <hamrachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 22:06:03 by hamrachi          #+#    #+#             */
/*   Updated: 2025/07/15 22:06:05 by hamrachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd) : sockfd(fd) {}

int Client::getSockfd() const
{
    return sockfd;
}

const std::string &Client::getNickname() const
{
    return nickname;
}

const std::string &Client::getUsername() const
{
    return username;
}

void Client::setNickname(const std::string &nick)
{
    nickname = nick;
}

void Client::setUsername(const std::string &user)
{
    username = user;
}
