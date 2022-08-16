/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Barney e Seus Amigos  <B.S.A@students>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 04:04:41 by Barney e Se       #+#    #+#             */
/*   Updated: 2022/08/16 14:31:04 by Barney e Se      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User( int userFd )
	: _userFd(userFd), _nick(""), _username(""), _auth( false ) {
	return ;
}

User::~User( void ) {
	return ;
}

void	User::receiveMessage( std::string msg ) {

	if (msg.find("\r\n") == std::string::npos)
		msg += "\r\n";

	if (send(getFd(), msg.c_str(), strlen(msg.c_str()), 0) < 0)
		Utils::errorMessager("receiveMessage: send:", strerror(errno));

	return ;

}

int	User::getFd( void ) {
	return (this->_userFd);
}

void	User::auth( void ) {
	this->_auth = true;
}

bool	User::isAuth( void ) {
	return (this->_auth);
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

std::string	User::getRealname( void ) {
	return (this->_realname);
}

void	User::setRealname( std::string realname ) {
	this->_realname = realname;
}
std::string	User::getServername( void ) {
	return (this->_servername);
}

void	User::setServername( std::string servername ) {
	this->_servername = servername;
}
std::string	User::getHostname( void ) {
	return (this->_hostname);
}

void	User::setHostname( std::string hostname ) {
	this->_hostname = hostname;
}
