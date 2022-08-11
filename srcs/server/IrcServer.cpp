// #include <IrcServer.hpp>
#include "../../includes/IrcServer.hpp"

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
 */
void	IrcServer::SetServerFd( void ) {
	struct addrinfo	*result;
	struct addrinfo	hints;
	int				ret;

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

	freeaddrinfo(result);
	std::cout << "SetServerFd called!" << std::endl;
	return ;
}