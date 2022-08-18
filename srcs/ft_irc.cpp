#include "IrcServer.hpp"

bool	loop;

void	handle_ctrl_c( int sig ) {

	(void)sig;

	std::cout << std::endl;
	std::cout << BGRN << "/ ***************************************** \\" << std::endl;
	std::cout <<         "|         😎  Thanks for User!!  😎         |" << std::endl;
	std::cout <<         "\\ ***************************************** /" << RESET <<std::endl;

	throw std::runtime_error("");

}

int	main( int argc, char **argv ) {

	if (argc != 3) {
		std::cerr << "usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	loop = true;
	signal(SIGINT, handle_ctrl_c);
	try {
		IrcServer	server(HOST, argv[1], argv[2]);

		server.initPoll();
	} catch ( std::exception & e ) {
		std::cerr << e.what() << std::endl;
	}

	return (0);

}
