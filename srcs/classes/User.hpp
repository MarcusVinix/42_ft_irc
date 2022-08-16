/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Barney e Seus Amigos <B.S.A@student>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 04:04:38 by Barney e Se       #+#    #+#             */
/*   Updated: 2022/08/16 11:14:09 by Barney e Se      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP

#include "ft_irc.hpp"
#include "Utils.hpp"
#include "Channel.hpp"

class Channel;

class User {

	private:

		int						_userFd;
		std::string				_nick;
		std::string				_username;
		std::string				_realname;
		std::string				_servername;
		std::string				_hostname;
		bool					_auth;
		std::vector<Channel *>	_channelsVec;

	public:

		User( int userFd );
		~User( void );

		void					receiveMessage( std::string msg );

		bool					isAuth( void );
		void					auth( void );
		int						getFd( void );

		std::string				getNick( void );
		void					setNick( std::string nick );
		std::string				getUsername( void );
		void					setUsername( std::string username );
		std::string				getRealname( void );
		void					setRealname( std::string realname );
		std::string				getServername( void );
		void					setServername( std::string servername );
		std::string				getHostname( void );
		void					setHostname( std::string hostname );
		std::vector<Channel *>	getChannels( void );
		void					addChannel( Channel * channel );
		void					removeChannel( Channel * channel );

};

#endif
