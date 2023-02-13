#include "Command.hpp"

NickCommand::NickCommand(Server *server, int auth) : Command(server, auth) {}

NickCommand::~NickCommand() {}

void NickCommand::execute(Client *client, std::vector<std::string> args)
{
	if (args.empty() || args.at(0).empty())
	{
		client->msgReply(ERR_NONICKNAMEGIVEN(client->getNickname()));
		return;
	}

	std::string	nickname = args.at(0);

	if (this->server->getClient(nickname))
	{
		client->msgReply(ERR_NICKNAMEINUSE(client->getNickname()));
		return;
	}
	client->setNickname(nickname);
	client->welcome();
}
