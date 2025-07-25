/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhamdou <ayhamdou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:56:03 by ayhamdou          #+#    #+#             */
/*   Updated: 2025/07/25 11:44:58 by ayhamdou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

# include <iostream>
# include <sys/socket.h>
# include <fcntl.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <vector>
# include <poll.h>
# include <unistd.h>
# include <stdio.h>

// # include "Server.hpp"

class Client
{
	private:
		int clientFd;
		std::string clientRealName;
		std::string clientNickName;
		std::string clientUserName;
		std::string clientHostName;
		std::string buffer;
		

	public:
		Client(int fd);
		~Client();
		int getClientFd();
		std::string getClientNickName();
		std::string getClientRealName();
		void appendToBuffer(const std::string& data);

		//tmp
		std::string getbuffer();
};


#endif