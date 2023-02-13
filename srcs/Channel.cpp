#include "Channel.hpp"

Channel::Channel(std::string const &name, std::string const &password)
{
	this->name = name;
	this->password = password;
}

Channel::~Channel() {}

std::vector<std::string>	Channel::getNicknames()
{
	std::vector<std::string>	nicknames;

	for (std::vector<Client *>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		nicknames.push_back((*it)->getNickname());
	}
	return (nicknames);
}

void	Channel::broadcast(std::string const &msg, Client *except)
{
	for (std::vector<Client *>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		if (*it == except)
			continue ;
		(*it)->reply(msg);
	}
}

void	Channel::addClient(Client *client)
{
	this->clients.push_back(client);
	if (this->clients.size() == 1)
		client->setStatus(2);
}

void	Channel::removeClient(Client *client)
{
	std::vector<Client *>::iterator	it;
	std::string						msg;

	// remove the client
	for (it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		if ((*it) == client)
		{
			this->clients.erase(it);
			break ;
		}
	}
	client->setChannel(nullp);
	// set status
	if (client->getStatus() == 2)
	{
		client->setStatus(1);
		if (this->clients.size() > 0)
		{
			this->clients.at(0)->setStatus(2);
			msg.append(this->clients.at(0)->getNickname()).append(" is now the admin of ").append(this->name);
			console_log(msg);
		}
	}
}

void	Channel::kick(Client *client, Client *target, std::string const &reason)
{
	std::string	tmp;

	this->broadcast(RPL_KICK(client->getPrefix(), this->name, target->getNickname(), reason));
	this->removeClient(target);

	tmp = client->getNickname() + " kicked " + target->getNickname() + " form channel " + this->name;
	console_log(tmp);
}
