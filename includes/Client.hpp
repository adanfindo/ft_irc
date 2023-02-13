#ifndef CLIENT_HPP
#define CLIENT_HPP

#pragma once

class Client;

#include "utils.hpp"
#include "Server.hpp"
#include "Channel.hpp"

class Client
{
	private:
		int			fd;
		std::string	hostname;
		int			port;

		std::string	nickname;
		std::string	username;
		std::string	realname;

		// 0: not logged
		// 1: logged
		// 2: admin
		int			status;
		Channel		*channel;

	public:
		Client(int fd, std::string hostname, int port);
		~Client();

		int			getFd() const { return (this->fd); }
		std::string	getHostname() const { return (this->hostname); }
		int			getPort() const { return (this->port); }
		std::string	getNickname() const { return (this->nickname); }
		std::string	getUsername() const { return (this->username); }
		std::string	getRealname() const { return (this->realname); }
		int			getStatus() const { return (this->status); }
		Channel		*getChannel() const { return (this->channel); }
		
		void		setFd(int fd) { this->fd = fd; }
		void		setHostname(std::string hostname) { this->hostname = hostname; }
		void		setPort(int port) { this->port = port; }
		void		setNickname(std::string nickname) { this->nickname = nickname; }
		void		setUsername(std::string username) { this->username = username; }
		void		setRealname(std::string realname) { this->realname = realname; }
		void		setStatus(int status) { this->status = status; }
		void		setChannel(Channel *channel) { this->channel = channel; }

		std::string	identify();
		std::string	log(std::string const &log);
		void		reply(std::string const &msg) const;
		void		msgReply(std::string const &msg);
		void		welcome();
		void		join(Channel *channel);
		void		leave();
		std::string	getPrefix() const;
};


#endif
