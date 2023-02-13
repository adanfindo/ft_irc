#include "Command.hpp"

QuitCommand::QuitCommand(Server *server, int auth) : Command(server, auth) {}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute(Client *client, std::vector<std::string> args)
{
	std::string	reason;

	if (args.empty())
		reason = "Leaving...";
	else
		reason = args.at(0);
	if (reason.at(0) == ':')
		reason = reason.substr(1);

	client->reply(RPL_QUIT(client->getPrefix(), reason));
}
