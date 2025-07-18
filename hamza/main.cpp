/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hamrachi <hamrachi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 22:09:45 by hamrachi          #+#    #+#             */
/*   Updated: 2025/07/17 01:31:57 by hamrachi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"

int main()
{
    Server server;

    // 1. إنشاء عميلين وهميين على fd 3 و 4
    server.addClient(3);
    server.addClient(4);

    // 2. البحث عنهم وتعيين الاسم والاسم المستعمل
    Client* alice = server.findClientByNickname(""); // أول واحد بدون اسم
    alice->setNickname("Alice");
    alice->setUsername("alice123");

    Client* bob = server.findClientByNickname(""); // الثاني بدون اسم
    bob->setNickname("Bob");
    bob->setUsername("bob456");

    // 3. Alice ترسل رسالة خاصة لـ Bob
    server.privateMessage(alice, "PRIVMSG Bob :Hello Bob! How are you?");

    return 0;
}
