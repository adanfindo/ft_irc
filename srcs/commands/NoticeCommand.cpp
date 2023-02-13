#include "Command.hpp"

NoticeCommand::NoticeCommand(Server *server, int auth) : Command(server, auth) {}

NoticeCommand::~NoticeCommand() {}

void	NoticeCommand::execute(Client *client, std::vector<std::string> args)
{
	if (args.size() < 2 || args.at(0).empty() || args.at(1).empty())
	{
		client->msgReply(ERR_NEEDMOREPARAMS(client->getNickname(), "NOTICE"));
		return;
	}

	std::string	target = args.at(0);
	std::string	msg;

	for (std::vector<std::string>::iterator it = args.begin() + 1; it != args.end(); ++it)
	{
		msg.append(*it + " ");
	}

	if (msg.at(0) == ':')
		msg = msg.substr(1);

	if (target.at(0) == '#')
	{

		Channel	*channel = client->getChannel();
		if (channel == nullp)
			return;

		channel->broadcast(RPL_NOTICE(client->getPrefix(), target, msg), client);
		return;
	}

	Client	*dst = this->server->getClient(target);
	if (dst == nullp)
	{
		client->msgReply(ERR_NOSUCHNICK(client->getNickname(), target));
		return;
	}

	dst->reply(RPL_NOTICE(client->getPrefix(), target, msg));
}
