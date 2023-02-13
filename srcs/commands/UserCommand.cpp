#include "Command.hpp"

UserCommand::UserCommand(Server *server, int auth) : Command(server, auth) {}

UserCommand::~UserCommand() {}

void UserCommand::execute(Client *client, std::vector<std::string> args)
{
	if (args.size() < 4)
	{
		client->msgReply(ERR_NEEDMOREPARAMS(client->getNickname(), "USER"));
		return;
	}
	client->setUsername(args.at(0));
	client->setRealname(args.at(3));
	client->welcome();
}
