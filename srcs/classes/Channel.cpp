/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Barney e Seus Amigos  <B.S.A@students>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 04:05:09 by Barney e Se       #+#    #+#             */
/*   Updated: 2022/08/17 17:04:40 by Barney e Se      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel( std::string name, std::string password )
	: _name(name), _password(password) {
	return ;
}

Channel::~Channel( void ) {
	return ;
}

std::string	Channel::getName( void ) {
	return (this->_name);
}

std::string	Channel::getPassword( void ) {
	return (this->_password);
}

std::vector<User *>	Channel::getUsers( void ) {
	return (this->_users);
}

User	*Channel::getUserByNick( std::string nick ) {

	std::vector<User *>::iterator	it = this->_users.begin();

	if (nick[0] == ':')
		nick.erase(0, 1);
	for ( ; it != this->_users.end(); it++) {
		if ((*it)->getNick() == nick)
			return (*it);
	}

	return (NULL);

}

void	Channel::addUser( User * user ) {

	if (getUserByNick(user->getNick()) == NULL)
		this->_users.push_back(user);

	return ;

}

void	Channel::removeUser( User * user ) {

	std::vector<User *>::iterator	it = this->_users.begin();

	for ( ; it != this->_users.end(); it ++) {
		if (*it == user) {
			this->_users.erase(it);
			return ;
		}
	}

	return ;

}

void	Channel::messageFromChannel( std::string msg ) {

	std::vector<User *>::iterator	it = this->_users.begin();

	if (msg.find("\r\n") == std::string::npos)
		msg += "\r\n";

	for ( ; it != this->_users.end(); it++)
		if (send((*it)->getFd(), msg.c_str(), strlen(msg.c_str()), 0) < 0)
			ft::errorMessage("messageFromChannel: send:", strerror(errno));

	return ;

}

void	Channel::messageToChannel( std::string msg, int senderFd ) {

	std::vector<User *>::iterator	it = this->_users.begin();

	if (msg.find("\r\n") == std::string::npos)
		msg += "\r\n";

	for ( ; it != this->_users.end(); it++)
		if ((*it)->getFd() != senderFd)
			if (send((*it)->getFd(), msg.c_str(), strlen(msg.c_str()), 0) < 0)
				ft::errorMessage("messageToChannel: send:", strerror(errno));

	return ;

}