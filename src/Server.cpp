/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhamdou <ayhamdou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:57:31 by ayhamdou          #+#    #+#             */
/*   Updated: 2025/07/01 16:01:14 by ayhamdou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{

}

Server::~Server()
{
	std::cout << "server clared";
}

//TODO - server init
/*
	Create a listening socket.

	Set the socket to non-blocking mode using fcntl.

	Use bind() and listen().

	Use poll() to wait for events (read, write, new connections).

	Accept new clients when the listening socket is ready.

	Read/write data on other sockets.
*/
//TODO - add protections


void Server::StartServer(int port)
{
	//init
	int socketFd = socket(AF_INET, SOCK_STREAM, 0); 
	if (socketFd < 0)
		perror("socket");
	int opt = 1;
	setsockopt(socketFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	fcntl(socketFd, F_SETFL, O_NONBLOCK);
	
	//binding
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(socketFd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		perror("bind");

	//start lestening
	if (listen(socketFd, SOMAXCONN) < 0)
		perror("lestening");

	//main loop
	std::vector<struct pollfd> poll_fds;
	struct pollfd pfd;
	pfd.fd = socketFd;
	pfd.events = POLLIN;
	poll_fds.push_back(pfd);

	while (1)
	{
		if (poll(&poll_fds[0], poll_fds.size(), -1) < 0)
		{
			perror("poll");
			break;
		}
		for (size_t i = 0; i < poll_fds.size(); i++)
		{
			if (poll_fds[i].revents & POLLIN)
			{
				if (poll_fds[i].fd == socketFd)
				{
					//new conection
					int client_fd = accept(socketFd, NULL, NULL);
					if (client_fd != -1)
					{
						fcntl(client_fd, F_SETFL, O_NONBLOCK);
						struct pollfd newfd;
						newfd.fd = client_fd;
						newfd.events = POLLIN;
						//new client
						poll_fds.push_back(newfd);
					}
				}
				else
				{
					char b[512];
					int byts = recv(poll_fds[i].fd, b, sizeof(b) - 1, 0);
					if (byts <= 0)
					{
						close(poll_fds[i].fd);
						poll_fds.erase(poll_fds.begin() + i);
						--i;
					}
					else
					{
						b[byts] = '\0';
						std::string msg(b);
						//parsing
						std::cout << "received " << msg << std::endl;
					}
				}
			}
		}
	}
}