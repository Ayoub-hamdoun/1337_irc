/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamrachi <hamrachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 22:05:19 by hamrachi          #+#    #+#             */
/*   Updated: 2025/07/17 01:40:14 by hamrachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>

class Client
{
private:
    int sockfd;
    std::string nickname;
    std::string username;

public:
    Client(int fd);

    // Getters
    int getSockfd() const;
    const std::string &getNickname() const;
    const std::string &getUsername() const;

    // Setters
    void setNickname(const std::string &nick);
    void setUsername(const std::string &user);
};

#endif
