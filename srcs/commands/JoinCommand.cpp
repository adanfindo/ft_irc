#include "Command.hpp"

JoinCommand::JoinCommand(Server *server, int auth) : Command(server, auth) {}

JoinCommand::~JoinCommand() {}

void	JoinCommand::execute(Client *client, std::vector<std::string> args)
{
	if (args.empty())
	{
		client->msgReply(ERR_NEEDMOREPARAMS(client->getNickname(), "PASS"));
		return ;
	}
	std::string	name = args.at(0);
	std::string	password = "";
	if (args.size() > 1)
		password = args.at(1);

	Channel	*channel = client->getChannel();
	if (channel != nullp)
	{
		client->msgReply(ERR_TOOMANYCHANNELS(client->getNickname(), name));
		return ;
	}

	channel = this->server->getChannel(name);
	if (channel == nullp)
		channel = this->server->createChannel(name, password);

	if (password.compare(channel->getPassword()))
	{
		client->msgReply(ERR_BADCHANNELKEY(client->getNickname(), name));
		return ;
	}

	client->join(channel);
}
