#include "Command.hpp"

PingCommand::PingCommand(Server *server, int auth) : Command(server, auth) {}

PingCommand::~PingCommand() {}

void	PingCommand::execute(Client *client, std::vector<std::string> args)
{
	if (args.empty())
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PING"));
		return ;
	}
	client->reply(RPL_PING(client->getPrefix(), args.at(0)));
}
