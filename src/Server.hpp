/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhamdou <ayhamdou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:57:34 by ayhamdou          #+#    #+#             */
/*   Updated: 2025/07/01 15:48:32 by ayhamdou         ###   ########.fr       */
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

class Server
{
	private:
		int port;
		std::string password;
	
	public:
		Server();
		~Server();
		// Server(int port);
		void StartServer(int port);
		

};

#endif