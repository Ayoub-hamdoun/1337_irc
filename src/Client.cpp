/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhamdou <ayhamdou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:55:39 by ayhamdou          #+#    #+#             */
/*   Updated: 2025/07/27 21:50:18 by ayhamdou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client(int fd)
{
	this->clientFd = fd;
}

Client::~Client()
{
	close(clientFd);
	std::cout << "Client disconnected: " << clientFd << std::endl;
}

int Client::getClientFd()
{
	return clientFd;
}

std::string Client::getClientRealName()
{
	return clientRealName;
}

std::string Client::getClientNickName()
{
	return clientNickName;
}

std::string Client::getbuffer()
{
	return buffer;
}

void Client::appendToBuffer(const std::string& data)
{
	this->buffer += data;
}

bool Client::getisRegistered()
{
	return isRegistered;
}