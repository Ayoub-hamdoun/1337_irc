/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhamdou <ayhamdou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:57:31 by ayhamdou          #+#    #+#             */
/*   Updated: 2025/07/26 20:36:17 by ayhamdou         ###   ########.fr       */
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



void Server::newConnection()
{
	int clientFd = accept(poll_fds[0].fd, NULL, NULL);
	if (clientFd < 0)
		throw std::runtime_error("accept failed");
	else
	{
		fcntl(clientFd, F_SETFL, O_NONBLOCK);
		struct pollfd newPollFd;
		newPollFd.fd = clientFd;
		newPollFd.events = POLLIN;
		newPollFd.revents = 0;
		poll_fds.push_back(newPollFd);
		Client *newClient = new Client(clientFd);
		clientList.push_back(newClient);
	}
}

void Server::handleClient(size_t i)
{
	char buffer[512];
	memset(buffer, 0, sizeof(buffer));
	Client *client = clientList[i - 1];
	int bytesReceived = recv(poll_fds[i].fd, buffer, sizeof(buffer), 0);
	if (bytesReceived < 0)
		return ; // throw exeption
	if (!bytesReceived)
	{
		std::string nickname = client->getClientNickName();
		std::cout << "Client disconnected: " << nickname << std::endl;
		//leave from all channels
		close(client->getClientFd());
		clientList.erase(clientList.begin() + i - 1);
		poll_fds.erase(poll_fds.begin() + i);
		delete client;
	}
	std::string msg(buffer, bytesReceived);
	if (msg.find('\n') == std::string::npos)
	{
		client->appendToBuffer(msg);
		return;
	}
	else
	{
		client->appendToBuffer(msg);
		std::vector<std::string> splittedMessages;
		splittedMessages = ft_splitIt(client->getbuffer());
		for (size_t j = 0; j < splittedMessages.size(); j++)
			processCommand(j, client, splittedMessages[j]);
	}
}

void Server::processCommand(size_t i, Client *client, const std::string &msg)
{
	(void) i; // Unused parameter
	if (msg.empty())
	{
		perror("Empty message received");
		exit(EXIT_FAILURE);
	}
	std::istringstream iss(msg);
	std::string command;
	iss >> command;
	//check if not registred
	// if (!client->isRegistered() )
	// 	auth
	// else
	// executecommand(clinet, msg, command);
	if (client->isRegistered)
		executeCommand(client, msg, command);
	else
	{
		
	}
	std::cout << "Processing command: " << command << std::endl;
}



std::vector<std::string> Server::ft_splitIt(const std::string &input)
{
    std::vector<std::string> lines;
    size_t start = 0;
    size_t end;

    while ((end = input.find('\n', start)) != std::string::npos) {
        std::string line = input.substr(start, end - start);

        if (!line.empty()) {
            if (line.at(line.size() - 1) == '\r')
                line += '\n';
            else {
                line += '\r';
                line += '\n';
            }
        }
        else
            line = "\r\n";
        lines.push_back(line);
        start = end + 1;
    }

    if (start < input.length()) {
        std::string line = input.substr(start);
        if (!line.empty()) {
            if (line.at(line.size() - 1) == '\r') {
                line += '\n';
            } else if (line.at(line.size() - 1) != '\n') {
                line += '\r';
                line += '\n';
            }
        }
        lines.push_back(line);
    }

    return lines;
}

void Server::StartServer(int port)
{
	this->port = port;

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
	struct pollfd pfd;
	pfd.fd = socketFd;
	pfd.events = POLLIN;
	poll_fds.push_back(pfd);

	while (1)
	{
		int ret = poll(&poll_fds[0], poll_fds.size(), -1);
		if (ret < 0)
		{
			perror("poll");
			break;
		}
		if (poll_fds[0].revents & POLLIN)
			newConnection();
		else
		{
			for (size_t i = 0; i < poll_fds.size(); i++)
			{
				if (poll_fds[i].revents & POLLIN)
					handleClient(i);
			}
		}
		// if (poll(&poll_fds[0], poll_fds.size(), -1) < 0)
		// {
		// 	perror("poll");
		// 	break;
		// }
		// for (size_t i = 0; i < poll_fds.size(); i++)
		// {
		// 	if (poll_fds[i].revents & POLLIN)
		// 	{
		// 		if (poll_fds[i].fd == socketFd)
		// 		{
		// 			//new conection
		// 			int client_fd = accept(socketFd, NULL, NULL);
		// 			if (client_fd != -1)
		// 			{
		// 				fcntl(client_fd, F_SETFL, O_NONBLOCK);
		// 				struct pollfd newfd;
		// 				newfd.fd = client_fd;
		// 				newfd.events = POLLIN;
		// 				//new client
		// 				poll_fds.push_back(newfd);
		// 			}
		// 		}
		// 		else
		// 		{
		// 			char b[512];
		// 			int byts = recv(poll_fds[i].fd, b, sizeof(b) - 1, 0);
		// 			if (byts <= 0)
		// 			{
		// 				close(poll_fds[i].fd);
		// 				poll_fds.erase(poll_fds.begin() + i);
		// 				--i;
		// 			}
		// 			else
		// 			{
		// 				b[byts] = '\0';
		// 				std::string msg(b);
		// 				//parsing
		// 				std::cout << "received " << msg << std::endl;
		// 			}
		// 		}
		// 	}
		// }
	}
}