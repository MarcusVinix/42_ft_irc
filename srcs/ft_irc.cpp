#include "IrcServer.hpp"

int	loop = 1;

int	main( int argc, char **argv ) {

	if (argc != 3) {
		std::cerr << "usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	IrcServer server(HOST, argv[1], argv[2]);

	server.initPoll();

	return (0);

}
