/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayhamdou <ayhamdou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:53:02 by ayhamdou          #+#    #+#             */
/*   Updated: 2025/06/03 12:06:04 by ayhamdou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include <iostream>

int main(int argc, char const *argv[])
{
	(void) argv;
	if (argc != 3)
	{
		std::cerr << "invalide usage : ./ircserv <port> <password>" << std::endl;
		return 1;
	}
	return 0;
}
