#include <ft_irc.hpp>
#include <IrcServer.hpp>

int	loop = 1;

int main(int argc, char **argv) {
	std::cout << "Hello World" << std::endl;
	if (argc != 3) {
		std::cerr << "usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	IrcServer server(HOST, argv[1], argv[2]);

	server.initPoll();
	return (0);
}