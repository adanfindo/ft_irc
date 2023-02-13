#include "IrcBot.hpp"


int	main(int argc, char **argv)
{
	try
	{
		if (argc != 4)
		{
			std::cout << "Usage: ./ircbot <host> <port> <password>\n";
			return (1);
		}
		IrcBot bot(argv[1], argv[2], argv[3]);
		bot.start();
	}
	catch (BotQuitException &err)
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
