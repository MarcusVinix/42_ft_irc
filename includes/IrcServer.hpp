#ifndef IRC_SERVER_HPP
# define IRC_SERVER_HPP

#include <ft_irc.hpp>

extern int	loop;

class IrcServer {

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

	private:

		std::string			_host;
		std::string			_port;
		std::string			_password;
		int					_socket_fd;
		std::vector<pollfd>	_pollfd_vec;
};

#endif