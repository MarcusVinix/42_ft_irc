/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Barney e Seus Amigos  <B.S.A@students>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 04:04:51 by Barney e Se       #+#    #+#             */
/*   Updated: 2022/08/16 14:36:46 by Barney e Se      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IrcServer.hpp"

IrcServer::IrcServer( std::string host, std::string port, std::string password )
	: _host(host), _port(port), _password(password) {
	this->setSocketFd();
	return ;
}

IrcServer::~IrcServer( void ) {
	return ;
}

/**
 * @brief This function will set the socket_fd to o_nonblock
 * and init the poll that will receive the request.
 *
 * about fcntl:
 * https://man7.org/linux/man-pages/man2/fcntl.2.html
 * https://www.geeksforgeeks.org/non-blocking-io-with-pipes-in-c/
 *
 * about poll:
 * https://man7.org/linux/man-pages/man2/poll.2.html
 */
void	IrcServer::initPoll( void ) {

	struct pollfd					pollFd = { this->_socketFd, POLLIN, 0 };
	std::vector<pollfd>::iterator	it;

	if (fcntl(this->_socketFd, F_SETFL, O_NONBLOCK) == -1)
		Utils::errorMessager("fcntl:", strerror(errno));

	this->_pollFdVec.push_back(pollFd);
	std::cout << "IrcServer Listen at: \n" <<
		this->_host << ":" << this->_port << std::endl;

	while (LOOP) {
		it = this->_pollFdVec.begin();
		if (poll(&(*it), this->_pollFdVec.size(), 1000) == -1)
			Utils::errorMessager("poll:", strerror(errno));
		this->_checkPoll();
	}

}

void	IrcServer::messageAllUsers( std::string msg ) {

	std::vector<User *>::iterator	it = this->_usersVec.begin();

	if (msg.find("\r\n") == std::string::npos)
		msg +="\r\n";

	for( ; it != this->_usersVec.end(); it++)
		if (send((*it)->getFd(), msg.c_str(), strlen(msg.c_str()), 0) < 0)
			Utils::errorMessager("messageAllUsers: send:", strerror(errno));

	return ;

}

void	IrcServer::deleteUser( int fd ) {

	std::vector<User *>::iterator	it = this->_usersVec.begin();

	for ( ; it != this->_usersVec.end(); it++) {
		if ((*it)->getFd() == fd) {
			this->_usersVec.erase(it);
			break ;
		}
	}
}

int	IrcServer::getSocketFd( void ) {
	return (this->_socketFd);
}

/**
 * @brief This function will create the socket fd, configure it to be reuseable,
 * bind with the address get by the getaddrinfo function and set the listen backlog.
 *
 * about getaddrinfo:
 *  https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html
 * look for the example.
 *
 * about setsockopt:
 * https://man7.org/linux/man-pages/man3/setsockopt.3p.html
 * https://stackoverflow.com/questions/21515946/what-is-sol-socket-used-for
 *
 * about bind:
 * https://man7.org/linux/man-pages/man2/bind.2.html
 *
 * about listen:
 *  https://man7.org/linux/man-pages/man2/listen.2.html
 */
void	IrcServer::setSocketFd( void ) {

	struct addrinfo	*resultList;
	struct addrinfo	*lst;
	struct addrinfo	hints;
	int				exitCode;
	int				serverFd;
	int				value = 1;

	memset(&hints, 0, sizeof(hints));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;

	exitCode = getaddrinfo(this->_host.c_str(), this->_port.c_str(), &hints, &resultList);
	if (exitCode != 0)
		Utils::errorMessager("getaddrinfo:", gai_strerror(exitCode));

	lst = resultList;
	while (lst) {
		serverFd = socket(lst->ai_family, lst->ai_socktype, lst->ai_protocol);
		if (serverFd == -1)
			continue ;
		exitCode = setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value));
		if (exitCode != 0) {
			close(serverFd);
			freeaddrinfo(resultList);
			Utils::errorMessager("setsockopt:", gai_strerror(exitCode));
		}
		exitCode = bind(serverFd, lst->ai_addr, lst->ai_addrlen);
		if (exitCode == 0)
			break ;
		close(serverFd);
		lst = lst->ai_next;
	}

	freeaddrinfo(resultList);
	if (lst == NULL) {
		Utils::errorMessager("bind:", gai_strerror(exitCode));
	}

	exitCode = listen(serverFd, LISTEN_BACKLOG);
	if (exitCode == -1)
		Utils::errorMessager("listen:", gai_strerror(exitCode));

	this->_socketFd = serverFd;

	return ;
}

User	*IrcServer::getUserByFd( int fd ) {

	std::vector<User *>::iterator	it = this->_usersVec.begin();

	for ( ; it != this->_usersVec.end(); it++) {
		if ((*it)->getFd() == fd)
			return (*it);
	}
	return (NULL);

}

User	*IrcServer::getUserByNick( std::string nick ) {

	std::vector<User *>::iterator	it = this->_usersVec.begin();

	for ( ; it != this->_usersVec.end(); it++) {
		if ((*it)->getNick() == nick)
			return (*it);
	}
	return (NULL);

}

std::string	IrcServer::getPassword( void ) {
	return (this->_password);
}

std::vector<User *>	IrcServer::getUsers( void ) {
	return (this->_usersVec);
}

/**
 * @brief This function will check the poll fd vector,
 * checking the if revents and POLLIN is true.
 * If the fd off the poll is equal the server socket fd,
 * this mean that is a new connection(User), so we call createUser to create a new user.
 * Otherwise this is a existing user, so we call messageReceived to get the message(command).
 *
 */
void	IrcServer::_checkPoll( void ) {

	std::vector<pollfd>::iterator	it;

	for (it = this->_pollFdVec.begin(); it != this->_pollFdVec.end(); it++) {
		if (it->revents && POLLIN) {
			if (it->fd == this->_socketFd)
				this->_createUser();
			else
				this->_messageReceived(it->fd);
			break ;
		}
	}

}

/**
 * @brief This function create a new User(connection).
 * The new User is created passing a fd got by the accept function.
 * This fd will be set to o_nonblock using fcntl.
 * A new pollfd will be create.
 * The new pollfd and user will be added to their respective vector.
 * About accept:
 * https://man7.org/linux/man-pages/man2/accept4.2.html
 */
void	IrcServer::_createUser( void ) {

	int					userFd;
	User				*newUser;
	struct sockaddr_in	cli_addr;
	socklen_t			len;

	len = sizeof(cli_addr);
	userFd = accept(this->_socketFd, (struct sockaddr *)&cli_addr, &len);
	if (userFd < 0)
		Utils::errorMessager("accept:", strerror(errno));

	pollfd	userPollFd = { userFd, POLLIN, 0 };
	if (fcntl(userFd, F_SETFL, O_NONBLOCK) == -1)
		Utils::errorMessager("createUser: fcntl:", strerror(errno));

	newUser = new User(userFd);
	this->_usersVec.push_back(newUser);
	this->_pollFdVec.push_back(userPollFd);
	std::cout << "New User: " << userFd << std::endl;

	return ;

}

/**
 * @brief This function will read the fd and get the buffer(message).
 * About recv:
 * https://man7.org/linux/man-pages/man2/recv.2.html
 * @todo After read all the message(command) a function or class command will be called.
 * @param fd to be read.
 */
void	IrcServer::_messageReceived( int fd ) {

	char		buff;
	std::string	str;

	while (str.find("\n")) {
		if (recv(fd, &buff, 1, 0) < 0) {
			continue;
		}
		else {
			str += buff;
			if (str.find("\n") != std::string::npos) {
				std::cout << "fd: " << fd << "  -  msg: " << str << std::endl;
				Command command(str, fd, *this);
				break ;
			}
		}
	}

}
