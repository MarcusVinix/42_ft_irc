#include <IrcServer.hpp>

IrcServer::IrcServer( std::string host, std::string port, std::string password )
	: _host(host), _port(port), _password(password) {
	std::cout << "IrcServer Constructor" << std::endl;
	this->SetServerFd();
	return ;
}

IrcServer::~IrcServer( void ) {
	return ;
}

void	IrcServer::SetServerFd( void ) {
	std::cout << "SetServerFd called!" << std::endl;
	return ;
}