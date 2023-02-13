#ifndef COMMAND_HPP
#define COMMAND_HPP

#pragma once

class Command;

#include "utils.hpp"
#include "Server.hpp"
#include "Client.hpp"

class Command
{
	protected:
		Server	*server;
		int		auth; // requires auth = 0: no, 1: yes
	
	public:
		Command(Server *server, int auth = 0) { this->server = server; this->auth = auth; }
		virtual ~Command() {}

		int				getAuth() const { return (this->auth); }
		virtual void	execute(Client *client, std::vector<std::string> args) = 0;
};

class NoticeCommand : public Command
{
	public:
		NoticeCommand(Server *server, int auth = 0);
		~NoticeCommand();

		void execute(Client *client, std::vector<std::string> args);
};

class PrivMsgCommand : public Command
{
	public:
		PrivMsgCommand(Server *server, int auth = 0);
		~PrivMsgCommand();

		void execute(Client *client, std::vector<std::string> args);
};

class PartCommand : public Command
{
	public:
		PartCommand(Server *server, int auth = 0);
		~PartCommand();

		void execute(Client *client, std::vector<std::string> args);
};

class QuitCommand : public Command
{
	public:
		QuitCommand(Server *server, int auth = 0);
		~QuitCommand();

		void execute(Client *client, std::vector<std::string> args);
};

class JoinCommand : public Command
{
	public:
		JoinCommand(Server *server, int auth = 0);
		~JoinCommand();

		void execute(Client *client, std::vector<std::string> args);
};

class UserCommand : public Command
{
	public:
		UserCommand(Server *server, int auth = 0);
		~UserCommand();

		void execute(Client *client, std::vector<std::string> args);
};

class NickCommand : public Command
{
	public:
		NickCommand(Server *server, int auth = 0);
		~NickCommand();

		void execute(Client *client, std::vector<std::string> args);
};


class PassCommand : public Command
{
	public:
		PassCommand(Server *server, int auth = 0);
		~PassCommand();

		void execute(Client *client, std::vector<std::string> args);
};

class KickCommand : public Command
{
	public:
		KickCommand(Server *server, int auth = 1);
		~KickCommand();

		void execute(Client *client, std::vector<std::string> args);
};

class PingCommand : public Command
{
	public:
		PingCommand(Server *server, int auth = 0);
		~PingCommand();

		void execute(Client *client, std::vector<std::string> args);
};

class PongCommand : public Command
{
	public:
		PongCommand(Server *server, int auth = 0);
		~PongCommand();

		void execute(Client *client, std::vector<std::string> args);
};

#endif
