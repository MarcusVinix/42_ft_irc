#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "IrcServer.hpp"

class IrcServer;

class Command {

	private:

		User						&_user;
		IrcServer					&_ircServer;
		std::vector<std::string>	_args;
		std::string					_command;

	public:

		Command( std::string buffer, int userFd, IrcServer & ircServer );
		~Command( void );

		void						parserBuffer( std::string buffer );
		std::string					ft_toupper( std::string str );
		std::vector<std::string>	ft_split( std::string str, char c);

};

#endif