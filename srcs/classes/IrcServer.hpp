#ifndef IRC_SERVER_HPP
# define IRC_SERVER_HPP

#include "User.hpp"

class IrcServer {

	private:

		std::string			_host;
		std::string			_port;
		std::string			_password;
		int					_socketFd;
		std::vector<pollfd>	_pollFdVec;
		std::vector<User*>	_usersVec;

	public:

		IrcServer( std::string host, std::string port, std::string password );
		~IrcServer( void );

		/**
		 * @brief This function will create the socket fd.
		 * This socket fd will be used to create the server.
		 *
		 */
		void	setSocketFd( void );
		void	initPoll( void );
		void	checkPoll( void );
		void	messageReceived( int fd );
		void	createUser( void );

};

#endif
