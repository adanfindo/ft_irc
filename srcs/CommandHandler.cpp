#include "CommandHandler.hpp"

CommandHandler::CommandHandler(Server *server)
{
	this->server = server;

	this->commands["PASS"] = new PassCommand(server, 0);
	this->commands["NICK"] = new NickCommand(server, 0);
	this->commands["USER"] = new UserCommand(server, 0);
	this->commands["QUIT"] = new QuitCommand(server, 0);
	
	this->commands["PING"] = new PingCommand(server, 1);
	this->commands["PONG"] = new PongCommand(server, 1);
	this->commands["JOIN"] = new JoinCommand(server, 1);
	this->commands["PART"] = new PartCommand(server, 1);
	this->commands["PRIVMSG"] = new PrivMsgCommand(server, 1);
	this->commands["NOTICE"] = new NoticeCommand(server, 1);

	this->commands["KICK"] = new KickCommand(server, 2);
}

CommandHandler::~CommandHandler()
{
	std::map<std::string, Command *>::iterator	it;

	for (it = this->commands.begin(); it != this->commands.end(); ++it)
		delete it->second;
}

int	CommandHandler::handle_command(Client *client, std::string cmd)
{
	std::stringstream	ss_cmd(cmd);
	std::string			parsed;
	int					length;
	std::string			name;

	while (std::getline(ss_cmd, parsed))
	{
		std::string	msg;
		// parse lines
		length = parsed.length();
		if (parsed[parsed.length() - 1] == '\n')
			length -= 1; 
		parsed = parsed.substr(0, length);
		// parse command
		name = parsed.substr(0, parsed.find(' '));
		try
		{
			// get the command
			Command						*command = this->commands.at(name);
			std::vector<std::string>	args;
			std::stringstream			ss_args(parsed.substr(name.length(), parsed.length()));
			std::string					arg;

			// if he has no authorization
			if (client->getStatus() < command->getAuth()) // auth check
			{
				if (!client->getStatus())
					client->msgReply(ERR_NOTREGISTERED(client->getNickname()));
				else
					client->msgReply(ERR_CHANOPRIVSNEEDED(client->getNickname(), client->getChannel()->getName()));
				return (0);
			}
			// get the args
			while (ss_args >> arg)
				args.push_back(arg);
			// execute the command
			command->execute(client, args);
		}
		catch (const std::out_of_range &err)
		{
			client->msgReply(ERR_UNKNOWNCOMMAND(client->getNickname(), name));
		}

		if (!name.compare("QUIT"))
			return (1);
	}
	return (0);
}
