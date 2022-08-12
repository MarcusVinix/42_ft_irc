#include <User.hpp>

User::User( int user_fd )
	: _user_fd(user_fd), _nick(""), _username(""), _password("") {
	return ;
}

User::~User( void ) {
	return ;
}

void	User::setNick( std::string nick ) {
	this->_nick = nick;
	return ;
}

void	User::setUsername( std::string username ) {
	this->_username = username;
	return ;
}

void	User::setPassword( std::string password ) {
	this->_password = password;
	return ;
}

std::string	User::getNick( void ) {
	return (this->_nick);
}

std::string	User::getUsername( void ) {
	return (this->_username);
}

std::string	User::getPassword( void ) {
	return (this->_password);
}