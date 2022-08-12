#ifndef USER_HPP
# define USER_HPP

#include "ft_irc.hpp"

class User {

	public:

		User( int user_fd );
		~User( void );

		void		setNick( std::string nick );
		void		setUsername( std::string username );
		std::string	getNick( void );
		std::string	getUsername( void );
		

	private:

		int			_user_fd;
		std::string	_nick;
		std::string	_username;

};

#endif