#include "Server.hpp"


int	main(int argc, char **argv)
{
	try
	{
		if (argc != 3)
		{
			std::cout << "Usage: ./ircserv <port> <password>\n";
			return (1);
		}
		Server server(argv[1], argv[2]);
		server.start();
	}
	catch (ServerQuitException &err)
	{
		(void)err;
	}
	catch (std::exception &err)
	{
		std::cout << "Error: " << err.what() << "\n";
		return (1);
	}
	return (0);
}
