#include <IrcServer.hpp>
// #include "../../includes/IrcServer.hpp"

IrcServer::IrcServer( std::string host, std::string port, std::string password )
	: _host(host), _port(port), _password(password) {
	std::cout << "IrcServer Constructor" << std::endl;
	this->SetServerFd();
	return ;
}

IrcServer::~IrcServer( void ) {
	return ;
}

/**
 * @brief 
 * about getaddrinfo:
 *  https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html
 * look for the example.
 * 
 * about setsockopt:
 * https://man7.org/linux/man-pages/man3/setsockopt.3p.html
 * 
 * about bind:
 * https://man7.org/linux/man-pages/man2/bind.2.html
 * 
 * about listen:
 *  https://man7.org/linux/man-pages/man2/listen.2.html
 */
void	IrcServer::SetServerFd( void ) {
	struct addrinfo	*result, *res_list, hints;
	int				ret, server_fd, value = 1;

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;

	ret = getaddrinfo(this->_host.c_str(), this->_port.c_str(), &hints, &result);
	if (ret != 0) {
		std::cerr << "getaddrinfo: " << gai_strerror(ret) << std::endl;
		exit(EXIT_FAILURE);
	}

	res_list = result;
	while (res_list) {
		server_fd = socket(res_list->ai_family, res_list->ai_socktype, res_list->ai_protocol);
		if (server_fd == -1)
			continue ;
		ret = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));
		if (ret != 0) {
			close(server_fd);
			freeaddrinfo(result);
			std::cerr << "setsockopt: " << gai_strerror(ret) << std::endl;
			exit(EXIT_FAILURE);
		}
		ret = bind(server_fd, res_list->ai_addr, res_list->ai_addrlen);
		if (ret == 0)
			break ;
		close(server_fd);
		res_list = res_list->ai_next;
	}
	freeaddrinfo(result);
	if (res_list == NULL) {
		std::cerr << "bind: " << gai_strerror(ret) << std::endl;
		exit(EXIT_FAILURE);
	}
	ret = listen(server_fd, LISTEN_BACKLOG);
	if (ret == -1) {
		std::cerr << "listen: " << gai_strerror(ret) << std::endl;
		exit(EXIT_FAILURE);
	}
	this->_server_fd = server_fd;
	return ;
}