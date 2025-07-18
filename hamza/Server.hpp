/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamrachi <hamrachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 22:07:07 by hamrachi          #+#    #+#             */
/*   Updated: 2025/07/15 22:07:19 by hamrachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <map>
#include "Client.hpp"

class Server
{
private:
    std::map<int, Client*> clients; // sockfd --> Client*

public:
    Server();
    ~Server();

    // Functions to handle private messages
    void privateMessage(Client *client, const std::string &line);

    // Helper functions
    Client *findClientByNickname(const std::string &nickname);
    void sendReply(int fd, const std::string &msg);

    // Functions to manage clients
    void addClient(int fd);
};

#endif
