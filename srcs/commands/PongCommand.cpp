#include "Command.hpp"

PongCommand::PongCommand(Server *server, int auth) : Command(server, auth) {}

PongCommand::~PongCommand() {}

void	PongCommand::execute(Client *client, std::vector<std::string> args)
{
	if (args.empty())
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PONG"));
		return ;
	}
	client->reply(RPL_PING(client->getPrefix(), args.at(0)));
}
