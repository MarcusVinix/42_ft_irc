/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IrcServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Barney e Seus Amigos  <B.S.A@students>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 04:04:46 by Barney e Se       #+#    #+#             */
/*   Updated: 2022/08/16 04:04:47 by Barney e Se      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

	private:

		void				_checkPoll( void );
		void				_createUser( void );
		void				_messageReceived( int fd );

	public:

		void				initPoll( void );
		void				messageAllUsers( std::string msg );

		int					getSocketFd( void );
		void				setSocketFd( void );
		User				*getUserByFd( int fd );
		User				*getUserByNick( std::string nick );
		std::string			getPassword( void );
		std::vector<User *>	getUsers( void );

};

#endif
