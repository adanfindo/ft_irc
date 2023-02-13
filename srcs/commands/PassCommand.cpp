#include "Command.hpp"

PassCommand::PassCommand(Server *server, int auth) : Command(server, auth) {}

PassCommand::~PassCommand() {}

void	PassCommand::execute(Client *client, std::vector<std::string> args)
{
	if (client->getStatus() != 0)
	{
		client->reply(ERR_ALREADYREGISTERED(client->getNickname()));
		return ;
	}
	if (args.empty())
	{
		client->reply(ERR_NEEDMOREPARAMS(client->getNickname(), "PASS"));
		return ;
	}
	if (args.at(0).substr(args.at(0).at(0) == ':' ? 1 : 0).compare(this->server->getPassword()))
	{
		client->reply(ERR_PASSWDMISMATCH(client->getNickname()));
		return ;
	}
	client->setStatus(1);
}
