#include "Client.hpp"

Client::Client(int fd, std::string hostname, int port)
{
	this->fd = fd;
	this->hostname = hostname;
	this->port = port;
	this->status = 0;
	this->channel = nullp;
}

Client::~Client() {}

std::string	Client::identify()
{
	std::string	msg;
	char		tmp[1000];

	sprintf(tmp, "%s:%d", this->getHostname().c_str(), this->getPort());
	msg.append(tmp);
	return (msg);
}

std::string	Client::log(std::string const &log)
{
	std::string	msg;
	char		tmp[1000];

	sprintf(tmp, "%s %s", this->identify().c_str(), log.c_str());
	msg.append(tmp);
	return (msg);
}

void	Client::reply(std::string const &msg) const
{
	if (DEBUG)
		console_log(msg);
	std::string	tmp = msg + "\r\n";
	if (send(this->fd, tmp.c_str(), tmp.length(), 0) < 0)
		throw std::runtime_error("Error while sending");
}

void	Client::msgReply(std::string const &msg)
{
	if (DEBUG)
		console_log(msg);
	this->reply(":" + this->getPrefix() + " " + msg);
}

void	Client::welcome()
{
	if (!this->status || this->username.empty() || this->realname.empty() || this->nickname.empty())
		return ;
	this->msgReply(RPL_WELCOME(this->nickname));
	console_log(this->log("is known as " + this->nickname));
}

void	Client::join(Channel *channel)
{
	std::string					users_string;
	std::vector<std::string>	users;

	channel->addClient((this));
	this->channel = channel;
	users = channel->getNicknames();
	for (std::vector<std::string>::iterator it = users.begin(); it != users.end(); ++it)
		users_string.append((*it)).append(" ");
	
	this->msgReply(RPL_NAMREPLY(this->nickname, channel->getName(), users_string));
	this->msgReply(RPL_ENDOFNAMES(this->nickname, channel->getName()));
	
	channel->broadcast(RPL_JOIN(this->getPrefix(), channel->getName()));
	console_log(this->nickname + " has joined channel " + channel->getName());
}

void	Client::leave()
{
	if (this->channel == nullp)
		return ;
	this->channel->broadcast(RPL_PART(this->getPrefix(), this->channel->getName()));
	console_log(this->nickname + " has left channel " + channel->getName());
	this->channel->removeClient((this));
}

std::string	Client::getPrefix() const
{
	std::string	tmp;

	tmp = this->nickname;
	if (!this->username.empty())
		tmp.append("!").append(this->username);
	if (!this->hostname.empty())
		tmp.append("@").append(this->hostname);
	return (tmp);
}
