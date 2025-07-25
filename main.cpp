#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

class Channel
{
private:
    std::string             name;
    std::string             topic;
    std::string             key;
    std::vector<std::string> operators;
    std::vector<std::string> users;
    bool                    invite_only;
    bool                    topic_restricted;
    int                     user_limit;

public:
    Channel(const std::string &channel_name) : name(channel_name), topic(""), key(""), 
            invite_only(false), topic_restricted(false), user_limit(-1) {}

    const std::string &getName() const
    {
        return (name);
    }

    const std::string &getTopic() const
    {
        return (topic);
    }

    void setTopic(const std::string &new_topic)
    {
        topic = new_topic;
    }

    // Mode getters
    bool isInviteOnly() const { return invite_only; }
    bool isTopicRestricted() const { return topic_restricted; }
    const std::string &getKey() const { return key; }
    int getUserLimit() const { return user_limit; }

    // Mode setters
    void setInviteOnly(bool mode) { invite_only = mode; }
    void setTopicRestricted(bool mode) { topic_restricted = mode; }
    void setKey(const std::string &new_key) { key = new_key; }
    void removeKey() { key = ""; }
    void setUserLimit(int limit) { user_limit = limit; }
    void removeUserLimit() { user_limit = -1; }

    bool removeOperator(const std::string &user)
    {
        for (size_t i = 0; i < operators.size(); i++)
        {
            if (operators[i] == user)
            {
                operators.erase(operators.begin() + i);
                return true;
            }
        }
        return false;
    }

    bool canJoin(const std::string &provided_key = "") const
    {
        // Check user limit
        if (user_limit > 0 && (int)users.size() >= user_limit)
            return false;
        
        // Check key
        if (!key.empty() && provided_key != key)
            return false;
        
        return true;
    }

    void addOperator(const std::string &op)
    {
        operators.push_back(op);
        addUser(op);
    }

    void addUser(const std::string &user)
    {
        for (size_t i = 0; i < users.size(); i++)
        {
            if (users[i] == user)
                return;
        }
        // Check if channel allows this user to join
        if (invite_only && !isOperator(user))
        {
            // In invite-only mode, only operators can add users directly
            // or users must be explicitly invited
            return;
        }
        users.push_back(user);
    }

    bool isOperator(const std::string &user) const
    {
        for (size_t i = 0; i < operators.size(); i++)
        {
            if (operators[i] == user)
                return (true);
        }
        return (false);
    }

    bool hasUser(const std::string &user) const
    {
        for (size_t i = 0; i < users.size(); i++)
        {
            if (users[i] == user)
                return (true);
        }
        return (false);
    }

    bool kickUser(const std::string &user)
    {
        for (size_t i = 0; i < users.size(); i++)
        {
            if (users[i] == user)
            {
                users.erase(users.begin() + i);
                return (true);
            }
        }
        return (false);
    }
};

class IRC
{
private:
    std::vector<Channel> channels;

public:
    void addChannel(const Channel &channel)
    {
        channels.push_back(channel);
    }

    Channel *findChannel(const std::string &name)
    {
        for (size_t i = 0; i < channels.size(); i++)
        {
            if (channels[i].getName() == name)
                return (&channels[i]);
        }
        return (NULL);
    }

    void invite(const std::string &requester, const std::string &channel_name,
                const std::string &target_user)
    {
        Channel *channel = findChannel(channel_name);
        if (!channel)
        {
            std::cout << "❌ Channel " << channel_name << " does not exist.\n";
            return;
        }
        if (!channel->isOperator(requester))
        {
            std::cout << "❌ You (" << requester << ") are not an operator in "
                      << channel_name << ".\n";
            return;
        }
        if (channel->hasUser(target_user))
        {
            std::cout << "⚠️ User " << target_user << " is already in "
                      << channel_name << ".\n";
            return;
        }
        channel->addUser(target_user);
        std::cout << "✅ " << requester << " invited " << target_user << " to "
                  << channel_name << ".\n";
    }

    void kick(const std::string &requester, const std::string &channel_name,
              const std::string &target_user)
    {
        Channel *channel = findChannel(channel_name);
        if (!channel)
        {
            std::cout << "❌ Channel " << channel_name << " does not exist.\n";
            return;
        }
        if (!channel->isOperator(requester))
        {
            std::cout << "❌ You (" << requester << ") are not an operator in "
                      << channel_name << ".\n";
            return;
        }
        if (!channel->hasUser(target_user))
        {
            std::cout << "❌ User " << target_user << " is not in channel "
                      << channel_name << ".\n";
            return;
        }
        if (channel->kickUser(target_user))
        {
            std::cout << "✅ " << requester << " kicked " << target_user
                      << " from " << channel_name << ".\n";
        }
        else
        {
            std::cout << "❌ Failed to kick user " << target_user << ".\n";
        }
    }

    void view_topic(const std::string &requester, const std::string &channel_name)
    {
        Channel *channel = findChannel(channel_name);
        if (!channel)
        {
            std::cout << "❌ Channel " << channel_name << " does not exist.\n";
            return;
        }
        if (!channel->hasUser(requester))
        {
            std::cout << "❌ You (" << requester << ") are not in channel "
                      << channel_name << ".\n";
            return;
        }
        
        std::string topic = channel->getTopic();
        if (topic.empty())
        {
            std::cout << "📋 No topic is set for " << channel_name << ".\n";
        }
        else
        {
            std::cout << "📋 Topic for " << channel_name << ": " << topic << "\n";
        }
    }

    void set_topic(const std::string &requester, const std::string &channel_name, 
                   const std::string &new_topic)
    {
        Channel *channel = findChannel(channel_name);
        if (!channel)
        {
            std::cout << "❌ Channel " << channel_name << " does not exist.\n";
            return;
        }
        
        // Check if topic is restricted to operators
        if (channel->isTopicRestricted() && !channel->isOperator(requester))
        {
            std::cout << "❌ You (" << requester << ") are not an operator in "
                      << channel_name << ". Topic is restricted to operators.\n";
            return;
        }
        
        if (!channel->hasUser(requester))
        {
            std::cout << "❌ You (" << requester << ") are not in channel "
                      << channel_name << ".\n";
            return;
        }
        
        channel->setTopic(new_topic);
        std::cout << "✅ " << requester << " set topic for " << channel_name 
                  << " to: " << new_topic << "\n";
    }

    void mode(const std::string &requester, const std::string &channel_name, 
              const std::string &modes, const std::string &param = "")
    {
        Channel *channel = findChannel(channel_name);
        if (!channel)
        {
            std::cout << "❌ Channel " << channel_name << " does not exist.\n";
            return;
        }
        
        if (!channel->isOperator(requester))
        {
            std::cout << "❌ You (" << requester << ") are not an operator in "
                      << channel_name << ".\n";
            return;
        }

        if (modes.empty())
        {
            // Show current modes
            std::cout << "🔧 Current modes for " << channel_name << ": ";
            std::string current_modes = "+";
            if (channel->isInviteOnly()) current_modes += "i";
            if (channel->isTopicRestricted()) current_modes += "t";
            if (!channel->getKey().empty()) current_modes += "k";
            if (channel->getUserLimit() > 0) current_modes += "l";
            
            if (current_modes == "+") current_modes = "No modes set";
            std::cout << current_modes << "\n";
            return;
        }

        bool adding = true;
        for (size_t i = 0; i < modes.length(); i++)
        {
            char c = modes[i];
            if (c == '+')
            {
                adding = true;
                continue;
            }
            else if (c == '-')
            {
                adding = false;
                continue;
            }

            switch (c)
            {
                case 'i': // invite-only
                    channel->setInviteOnly(adding);
                    std::cout << "🔧 " << requester << " " << (adding ? "set" : "removed") 
                              << " invite-only mode for " << channel_name << "\n";
                    break;
                    
                case 't': // topic restricted
                    channel->setTopicRestricted(adding);
                    std::cout << "🔧 " << requester << " " << (adding ? "set" : "removed") 
                              << " topic restriction for " << channel_name << "\n";
                    break;
                    
                case 'k': // key (password)
                    if (adding)
                    {
                        if (param.empty())
                        {
                            std::cout << "❌ Key parameter required for +k mode\n";
                            continue;
                        }
                        channel->setKey(param);
                        std::cout << "🔧 " << requester << " set key for " << channel_name 
                                  << " to: " << param << "\n";
                    }
                    else
                    {
                        channel->removeKey();
                        std::cout << "🔧 " << requester << " removed key from " 
                                  << channel_name << "\n";
                    }
                    break;
                    
                case 'o': // operator privilege
                    if (param.empty())
                    {
                        std::cout << "❌ User parameter required for " << (adding ? "+o" : "-o") << " mode\n";
                        continue;
                    }
                    if (!channel->hasUser(param))
                    {
                        std::cout << "❌ User " << param << " is not in channel " 
                                  << channel_name << "\n";
                        continue;
                    }
                    if (adding)
                    {
                        if (!channel->isOperator(param))
                        {
                            channel->addOperator(param);
                            std::cout << "🔧 " << requester << " gave operator privilege to " 
                                      << param << " in " << channel_name << "\n";
                        }
                        else
                        {
                            std::cout << "⚠️ " << param << " is already an operator in " 
                                      << channel_name << "\n";
                        }
                    }
                    else
                    {
                        if (channel->removeOperator(param))
                        {
                            std::cout << "🔧 " << requester << " removed operator privilege from " 
                                      << param << " in " << channel_name << "\n";
                        }
                        else
                        {
                            std::cout << "❌ " << param << " is not an operator in " 
                                      << channel_name << "\n";
                        }
                    }
                    break;
                    
                case 'l': // user limit
                    if (adding)
                    {
                        if (param.empty())
                        {
                            std::cout << "❌ Limit parameter required for +l mode\n";
                            continue;
                        }
                        try
                        {
                            int limit = std::atoi(param.c_str());
                            if (limit <= 0)
                            {
                                std::cout << "❌ User limit must be positive\n";
                                continue;
                            }
                            channel->setUserLimit(limit);
                            std::cout << "🔧 " << requester << " set user limit for " 
                                      << channel_name << " to " << limit << "\n";
                        }
                        catch (const std::exception &e)
                        {
                            std::cout << "❌ Invalid limit parameter: " << param << "\n";
                        }
                    }
                    else
                    {
                        channel->removeUserLimit();
                        std::cout << "🔧 " << requester << " removed user limit from " 
                                  << channel_name << "\n";
                    }
                    break;
                    
                default:
                    std::cout << "❌ Unknown mode: " << c << "\n";
                    break;
            }
        }
    }
};

int main()
{
    IRC irc;

    Channel general("#general");
    general.addOperator("amine");
    general.addOperator("rafiq");
    general.addUser("walo");
    general.addUser("guest");

    Channel random("#random");
    random.addOperator("walo");
    random.addUser("amine");
    random.addUser("guest2");

    irc.addChannel(general);
    irc.addChannel(random);

    std::string line;
    while (true)
    {
        std::cout << "Enter command: ";
        std::getline(std::cin, line);
        if (line == "exit")
            break;

        std::istringstream iss(line);
        std::string requester, command, channel, target_user;
        iss >> requester >> command >> channel >> target_user;

        if (command == "kick")
        {
            if (channel.empty() || target_user.empty())
            {
                std::cout << "⚠️ Usage: <your_name> kick <#channel> <user>\n";
                continue;
            }
            irc.kick(requester, channel, target_user);
        }
        else if (command == "invite")
        {
            if (channel.empty() || target_user.empty())
            {
                std::cout << "⚠️ Usage: <your_name> invite <#channel> <user>\n";
                continue;
            }
            irc.invite(requester, channel, target_user);
        }
        else if (command == "topic")
        {
            if (channel.empty())
            {
                std::cout << "⚠️ Usage: <your_name> topic <#channel> [new_topic]\n";
                continue;
            }
            
            if (target_user.empty())
            {
                // View topic
                irc.view_topic(requester, channel);
            }
            else
            {
                // Set topic - need to get the rest of the line as topic
                std::string topic_text;
                size_t pos = line.find(target_user);
                if (pos != std::string::npos)
                {
                    topic_text = line.substr(pos);
                }
                irc.set_topic(requester, channel, topic_text);
            }
        }
        else if (command == "mode" || command == "Mode" || command == "MODE")
        {
            if (channel.empty())
            {
                std::cout << "⚠️ Usage: <your_name> mode <#channel> [+/-modes] [parameters]\n";
                std::cout << "📖 Available modes: i(invite-only), t(topic-restricted), k(key), o(operator), l(limit)\n";
                std::cout << "📋 Examples:\n";
                std::cout << "   " << requester << " mode #general +i    (set invite-only)\n";
                std::cout << "   " << requester << " mode #general -i    (remove invite-only)\n";
                std::cout << "   " << requester << " mode #general +k secret  (set key to 'secret')\n";
                std::cout << "   " << requester << " mode #general +o user123  (give operator to user123)\n";
                std::cout << "   " << requester << " mode #general +l 50  (set user limit to 50)\n";
                continue;
            }
            
            // Parse mode command
            std::string modes = target_user; // modes are in target_user position
            std::string param;
            
            // Get additional parameter if present
            std::istringstream mode_iss(line);
            std::string temp1, temp2, temp3, temp4, temp5;
            mode_iss >> temp1 >> temp2 >> temp3 >> temp4 >> temp5;
            if (!temp5.empty())
            {
                param = temp5;
            }
            
            irc.mode(requester, channel, modes, param);
        }
        else
        {
            std::cout << "Unknown command: " << command << std::endl;
        }
    }
    return (0);
}