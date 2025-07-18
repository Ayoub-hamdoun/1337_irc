#include "Server.hpp"
#include <sys/socket.h> // send
#include <unistd.h>     // close
#include <algorithm>    // std::find_if
#include <sstream>

Server::Server() {}

Server::~Server()
{
    for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
        delete it->second;
}

void Server::addClient(int fd)
{
    clients[fd] = new Client(fd);
}

Client *Server::findClientByNickname(const std::string &nickname)
{
    for (std::map<int, Client*>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->second->getNickname() == nickname)
            return it->second;
    }
    return NULL;
}

void Server::sendReply(int fd, const std::string &msg)
{
    //send(fd, msg.c_str(), msg.length(), 0);
    std::cout << "[send to fd " << fd << "] " << msg;
}

void Server::privateMessage(Client *client, const std::string &line)
{
    // parsing
    size_t pos = line.find(':');
    if (pos == std::string::npos)
    {
        sendReply(client->getSockfd(), "ERR_NOTEXTTOSEND :No text to send\r\n");
        return;
    }

    std::string target = line.substr(8, pos - 9);
    std::string message = line.substr(pos + 1);

    if (target.empty() || message.empty())
    {
        sendReply(client->getSockfd(), "ERR_NOTEXTTOSEND :No text to send\r\n");
        return;
    }

    Client *receiver = findClientByNickname(target);
    if (!receiver)
    {
        sendReply(client->getSockfd(), "ERR_NOSUCHNICK :No such nick\r\n");
        return;
    }

    // Send message to receiver
    std::ostringstream oss;
    oss << ":" << client->getNickname() << "!" << client->getUsername() << "@localhost PRIVMSG " 
        << target << " :" << message << "\r\n";


    sendReply(receiver->getSockfd(), oss.str());
}
