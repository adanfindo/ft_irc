#include "Command.hpp"

KickCommand::KickCommand(Server *server, int auth) : Command(server, auth) {}

KickCommand::~KickCommand() {}

void	KickCommand::execute(Client *client, std::vector<std::string> args)
{
	if ((int)args.size() < 2)
	{
		client->msgReply(ERR_NEEDMOREPARAMS(client->getNickname(), "KICK"));
		return ;
	}

	std::string	name = args.at(0);
	std::string	target = args.at(1);
	std::string	reason = "No reason specified";

	if (args.size() >= 3 && (args[2][0] != ':' || args[2].size() > 1))
	{
		reason = "";
		for (std::vector<std::string>::iterator it = args.begin() + 2; it != args.end(); it++)
			reason.append(*it + " ");
	}

	Channel	*channel = client->getChannel();
	if (channel == nullp || channel->getName() != name)
	{
		client->msgReply(ERR_NOTONCHANNEL(client->getNickname(), name));
		return ;
	}

	Client	*dst = this->server->getClient(target);
	if (dst == nullp)
	{
		client->msgReply(ERR_NOSUCHNICK(client->getNickname(), target));
		return;
	}

	if (dst->getChannel() == nullp || dst->getChannel() != channel)
	{
		client->msgReply(ERR_USERNOTINCHANNEL(client->getNickname(), dst->getNickname(), name));
		return;
	}

	channel->kick(client, dst, reason);
}
