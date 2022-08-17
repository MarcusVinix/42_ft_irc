#include "IrcServer.hpp"

void	handle_ctrl_c( int sig ) {

	(void)sig;
	std::cout << "oi chamou" << std::endl;
	throw std::exception();

}

int	main( int argc, char **argv ) {

	if (argc != 3) {
		std::cerr << "usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}
	signal(SIGINT, handle_ctrl_c);
	try {
		IrcServer	server(HOST, argv[1], argv[2]);

		server.initPoll();
	} catch ( std::exception & e ) {
		std::cerr << e.what() << std::endl;
	}

	return (0);

}
