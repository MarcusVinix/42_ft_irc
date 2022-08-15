#ifndef USER_HPP
# define USER_HPP

#include "ft_irc.hpp"

class User {

	private:

		int			_userFd;
		std::string	_nick;
		std::string	_username;
		std::string	_realname;
		std::string	_servername;
		std::string	_hostname;
		bool		_auth;

	public:

		User( int userFd );
		~User( void );

		void	receiveMessage( std::string msg );

		std::string	getNick( void );
		void		setNick( std::string nick );
		std::string	getUsername( void );
		void		setUsername( std::string username );
		std::string	getRealname( void );
		void		setRealname( std::string realname );
		std::string	getServername( void );
		void		setServername( std::string servername );
		std::string	getHostname( void );
		void		setHostname( std::string hostname );
		bool		isAuth( void );
		void		auth( void );
		int			getFd( void );


};

#endif
