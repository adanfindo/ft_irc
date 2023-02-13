#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#pragma once

class CommandHandler;

#include "utils.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Command.hpp"

class CommandHandler
{
	private:
		Server								*server;
		std::map<std::string, Command *>	commands;

	public:
		CommandHandler(Server *server);
		~CommandHandler();

		int		handle_command(Client *client, std::string cmd);

};

#endif
