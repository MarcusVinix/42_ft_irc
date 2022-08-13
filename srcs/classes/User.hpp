#ifndef USER_HPP
# define USER_HPP

#include "ft_irc.hpp"

class User {

	private:

		int			_userFd;
		std::string	_nick;
		std::string	_username;

	public:

		User( int userFd );
		~User( void );

		std::string	getNick( void );
		void		setNick( std::string nick );
		std::string	getUsername( void );
		void		setUsername( std::string username );
		int			getFd( void );

};

#endif
