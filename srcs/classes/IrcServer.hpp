#ifndef IRC_SERVER_HPP
# define IRC_SERVER_HPP

#include "User.hpp"
#include "Command.hpp"

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

		void				initPoll( void );
		void				checkPoll( void );
		void				messageReceived( int fd );
		void				messageAllUsers( std::string msg );
		void				createUser( void );
		User				*getUserByFd( int fd );
		User				*getUserByNick( std::string nick );
		void				setSocketFd( void );
		int					getSocketFd( void );
		std::string			getPassword( void );
		std::vector<User *>	getUsers( void );

};


#endif
