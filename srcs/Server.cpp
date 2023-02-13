#include "Server.hpp"

Server::Server(std::string const &port, std::string const &password)
{
	this->host = "127.0.0.1";
	this->port = port;
	this->password = password;
	this->running = 1;
	this->sock = this->create_socket();
	this->handler = new CommandHandler((this));
}

Server::~Server()
{
	std::vector<int> fds;

	// disconnect all clients
	for (std::map<int, Client *>::iterator it = this->clients.begin(); it != this->clients.end(); ++it)
		fds.push_back(it->second->getFd());
	for (int fd = 0; fd != (int)fds.size(); ++fd)
	{
		this->clients[fds[fd]]->msgReply("Shutting down the server\n");
		this->handle_disconnection(fds[fd]);
	}
	// delete all channels
	for (int channel = 0; channel != (int)this->channels.size(); ++channel)
		delete this->channels.at(channel);
	// clean memory
	delete this->handler;
	close(this->sock);
	console_log("Main Socket Closed");
}

void	handle_sigint(int sig)
{
	(void)sig;
	throw ServerQuitException();
}

void	Server::start()
{
	pollfd	server_fd = {this->sock, POLLIN, 0};
	poll_fds.push_back(server_fd);

	if (!MAC_OS)
		signal(SIGINT, handle_sigint);
	console_log("Server waiting for connections");

	while (this->running)
	{
		// waiting for events
		if (poll(poll_fds.begin().base(), poll_fds.size(), -1) < 0)
			throw std::runtime_error("Error while polling");
		// event handling
		for (std::vector<pollfd>::iterator it = poll_fds.begin(); it != poll_fds.end(); ++it)
		{
			// no events
			if (it->revents == 0)
				continue ;
			// on connect
			if ((it->revents & POLLIN) == POLLIN)
			{
				if (it->fd == this->sock)
				{
					this->handle_connection();
					break ;
				}
				if (this->handle_message(it->fd))
					break ;
			}
			// on disconnect
			if ((it->revents & POLLHUP) == POLLHUP)
			{
				this->handle_disconnection(it->fd);
				break ;
			}
		}
	}
}

int	Server::create_socket()
{
	int	sockfd;
	int	int_port;

	// open the socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		throw std::runtime_error("Error while opening socket");
	// set to non-blocking mode
	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("Error while setting socket to NON-BLOCKING");
	// setup the binding informations
	struct sockaddr_in addr = {};
	bzero((char *)&addr, sizeof(addr));
	std::istringstream(this->port) >> int_port;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(int_port);
	// bind the socket
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		throw std::runtime_error("Error while binding socket");
	// listen on the socket
	if (listen(sockfd, MAX_CONNECTIONS) < 0)
		throw std::runtime_error("Error while listening in socket");
	return (sockfd);
}

void	Server::handle_connection()
{
	int			fd;
	sockaddr_in	addr = {};
	socklen_t 	size;
	std::string	ip_addr;

	// accept connection
	size = sizeof(addr);
	fd = accept(this->sock, (sockaddr *)&addr, &size);
	if (fd < 0)
		throw std::runtime_error("Error while accepting new client");
	// save th client's fd
	pollfd	poll_fd = {fd, POLLIN, 0};
	this->poll_fds.push_back(poll_fd);
	// get client info
	ip_addr = inet_ntoa(addr.sin_addr);
	if (getsockname(fd, (struct sockaddr *)&addr, &size) != 0)
		throw std::runtime_error("Error while gathering client informations");
	// create a new client
	Client *new_client = new Client(fd, ip_addr, ntohs(addr.sin_port));
	this->clients.insert(std::make_pair(fd, new_client));
	// log new connection
	console_log(new_client->log("has connected"));
}

std::string	Server::recive(int fd)
{
	std::string	msg;
	char		buffer[100];

	bzero(buffer, 100);
	// recive until new line
	while (!std::strstr(buffer, "\n"))
	{
		bzero(buffer, 100);
		if (recv(fd, buffer, 100, 0) < 0)
		{
			if (errno != EWOULDBLOCK)
				throw std::runtime_error("Error while reciving from client");
		}
		// clear buffer means exit (UNIX)
		if (!buffer[0])
			return ("");
		msg.append(buffer);
	}
	return (msg);
}

int	Server::handle_message(int fd)
{
	std::string msg = this->recive(fd);
	if (DEBUG)
		console_log(msg);
	// if disconnected
	if (msg[0] == 0)
	{
		this->handle_disconnection(fd);
		return (1);
	}
	// command handling
	Client	*client = this->clients.at(fd);
	if (this->handler->handle_command(client, msg))
	{
		this->handle_disconnection(client->getFd());
		return (1);
	}
	return (0);
}

void	Server::handle_disconnection(int fd)
{
	try
	{
		Client	*client = this->clients.at(fd);

		// remove the client from the channel
		client->leave();
		// remove the client
		this->clients.erase(fd);
		for (std::vector<pollfd>::iterator it = poll_fds.begin(); it != poll_fds.end(); ++it)
		{
			if (it->fd != fd)
				continue ;
			this->poll_fds.erase(it);
			close(fd);
			break ;
		}
		// message of disconnection
		console_log(client->log("has disconnected"));
		delete client;
	}
	catch (std::out_of_range const &err) {}
}

Client	*Server::getClient(std::string const &name)
{
	std::map<int, Client *>::iterator it;

	for (it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		if (!name.compare(it->second->getNickname()))
			return (it->second);
	}
	return (nullp);
}

Channel	*Server::getChannel(std::string const &name)
{
	std::vector<Channel *>::iterator it;

	for (it = this->channels.begin(); it != this->channels.end(); ++it)
	{
		if (!name.compare(it.operator*()->getName()))
			return (it.operator*());
	}
	return (nullp);
}

Channel	*Server::createChannel(std::string const &name, std::string const &password)
{
	Channel	*channel = new Channel(name, password);
	this->channels.push_back(channel);
	return (channel);
}
