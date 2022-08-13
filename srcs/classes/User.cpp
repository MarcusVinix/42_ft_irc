#include "User.hpp"

User::User( int userFd )
	: _userFd(userFd), _nick(""), _username("") {
	return ;
}

User::~User( void ) {
	return ;
}

std::string	User::getNick( void ) {
	return (this->_nick);
}

void	User::setNick( std::string nick ) {
	this->_nick = nick;
}

std::string	User::getUsername( void ) {
	return (this->_username);
}

void	User::setUsername( std::string username ) {
	this->_username = username;
}

int	User::getFd( void ) {
	return (this->_userFd);
}