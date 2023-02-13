#include "Command.hpp"

PartCommand::PartCommand(Server *server, int auth) : Command(server, auth) {}

PartCommand::~PartCommand() {}

void	PartCommand::execute(Client *client, std::vector<std::string> args)
{
	if (args.empty())
	{
		client->msgReply(ERR_NEEDMOREPARAMS(client->getNickname(), "PART"));
		return;
	}

	std::string	name = args.at(0);

	Channel	*channel = this->server->getChannel(name);
	if (channel == nullp)
	{
		client->msgReply(ERR_NOSUCHCHANNEL(client->getNickname(), name));
		return;
	}

	if (client->getChannel() == nullp || client->getChannel()->getName().compare(name))
	{
		client->msgReply(ERR_NOTONCHANNEL(client->getNickname(), name));
		return;
	}

	client->leave();
}
