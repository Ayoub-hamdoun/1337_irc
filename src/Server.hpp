/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhamdou <ayhamdou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:57:34 by ayhamdou          #+#    #+#             */
/*   Updated: 2025/07/26 16:16:36 by ayhamdou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
#define SERVER_H

# include <iostream>
# include <sys/socket.h>
# include <fcntl.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <vector>
# include <poll.h>
# include <unistd.h>
# include <stdio.h>
# include <cstring>
# include <cstdlib>
# include <sstream>
# include "Client.hpp"

class Server
{
	private:
		int port;
		std::string password;
		std::vector<Client *> clientList;
		std::vector<struct pollfd> poll_fds;
	
	public:
		Server();
		~Server();
		// Server(int port);
		void StartServer(int port);
		void handleClient(size_t i);
		void newConnection();
		void processCommand(size_t j, Client *client, const std::string &command);
		//tmp - move to utils
		std::vector<std::string> ft_splitIt(const std::string &str);

};

#endif