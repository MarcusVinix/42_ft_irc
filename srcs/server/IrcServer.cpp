#include <IrcServer.hpp>
// #include "../../includes/IrcServer.hpp"

IrcServer::IrcServer( std::string host, std::string port, std::string password )
	: _host(host), _port(port), _password(password) {
	this->setSocketFd();
	return ;
}

/**
 * @brief Destroy the Irc Server:: Irc Server object
 * 
 */
IrcServer::~IrcServer( void ) {
	return ;
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
	this->_socket_fd = server_fd;
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
	struct pollfd	fd_poll = { this->_socket_fd, POLLIN, 0 };
	std::vector<pollfd>::iterator	it_fd_poll;

	if (fcntl(this->_socket_fd, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << "fcntl: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	this->_pollfd_vec.push_back(fd_poll);
	std::cout << "IrcServer Listen at: \n" <<
		this->_host << ":" << this->_port << std::endl;
	while (loop) {
		it_fd_poll = this->_pollfd_vec.begin();
		if (poll(&(*it_fd_poll), this->_pollfd_vec.size(), 1000) == -1) {
			std::cerr << "poll: " << strerror(errno) << std::endl;
			exit(EXIT_FAILURE);
		}
		this->checkPoll();
	}
}

/**
 * @brief This function will check the poll fd vector,
 * checking the if revents and POLLIN is true.
 * If the fd off the poll is equal the server socket fd, 
 * this mean that is a new connection(User), so we call createUser to create a new user.
 * Otherwise this is a existing user, so we call messageReceived to get the message(command).
 * 
 */
void	IrcServer::checkPoll( void ) {
	std::vector<pollfd>::iterator	it;


	for (it = this->_pollfd_vec.begin(); it != this->_pollfd_vec.end(); it++) {
		if (it->revents && POLLIN) {
			if (it->fd == this->_socket_fd)
				this->createUser();
			else
				this->messageReceived(it->fd);
			break ;
		}
	}
}

/**
 * @brief This function will read the fd and get the buffer(message).
 * About recv:
 * https://man7.org/linux/man-pages/man2/recv.2.html
 * @todo After read all the message(command) a function or class command will be called.
 * @param fd to be read.
 */
void	IrcServer::messageReceived( int fd ) {
	char				buff;
	std::string			str;

	while (str.find("\n")) {
		int	ret = recv(fd, &buff, 1, 0);
		if (ret < 0) {
			continue;
		}
		else {
			str += buff;
			if (str.find("\n") != std::string::npos) {
				std::cout << "fd: " << fd << "msg: " << str << std::endl;
				break ;
			}
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
void	IrcServer::createUser( void ) {
	int					user_fd;
	User				*newUser;
	struct sockaddr_in	cli_addr;
	socklen_t			len;

	len = sizeof(cli_addr);
	user_fd = accept(this->_socket_fd, (struct sockaddr *)&cli_addr, &len);
	if (user_fd < 0) {
		std::cerr << "accept: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	pollfd user_poll_fd = { user_fd, POLLIN, 0 };
	if (fcntl(user_fd, F_SETFL, O_NONBLOCK) == -1) {
		std::cerr << "createUser: fcntl: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	newUser = new User(user_fd);
	this->_users_vec.push_back(newUser);
	this->_pollfd_vec.push_back(user_poll_fd);
	std::cout << "New User: " << user_fd << std::endl;
	return ;
}