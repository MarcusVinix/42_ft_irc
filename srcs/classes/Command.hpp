/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Barney e Seus Amigos  <B.S.A@students>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 04:04:57 by Barney e Se       #+#    #+#             */
/*   Updated: 2022/08/18 15:38:17 by Barney e Se      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
# define COMMAND_HPP

#include "IrcServer.hpp"
#include "Utils.hpp"
#include "Channel.hpp"

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

		void	parserBuffer( std::string buffer );
		void	checkCommand( void );
		void	numericResponse( std::string msg, std::string code, int fd = 0, std::string opt = "" );

		void	commandPass( void );
		void	commandNick( void );
		void	commandUser( void );
		void	commandPrivmsg( void );
		void	commandQuit( void );
		void	commandJoin( void );
		void	commandOper( void );
		void	commandNotice( void );
		void	commandPart( void );
		void	commandWho( void );
		void	commandKick( void );
		void	commandHelp( void );
		void	commandNames( void );
		void	commandKill( void );
		void	commandMode( void );
		void	commandDie( void );

};

#endif
