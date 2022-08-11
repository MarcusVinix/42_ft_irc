#ifndef IRC_SERVER_HPP
# define IRC_SERVER_HPP

#include <iostream>
#include <sys/socket.h>

class IrcServer {

	public:

		IrcServer( std::string host, std::string port, std::string password );
		~IrcServer( void );
		/**
		 * @brief This function will create the socket fd.
		 * This socket fd will be used to create the server.
		 * 
		 */
		void SetServerFd( void );

	private:

		std::string	_host;
		std::string	_port;
		std::string	_password;
		int			_server_fd;
};

#endif