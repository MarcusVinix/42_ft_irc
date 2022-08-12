#include <ft_irc.hpp>
#include <IrcServer.hpp>

int main(int argc, char **argv) {
	std::cout << "Hello World" << std::endl;
	if (argc != 3) {
		std::cerr << "usage: ./ircserv <port> <password>" << std::endl;
	}

	IrcServer server(HOST, argv[1], argv[2]);

	return (0);
}