/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Barney e Seus Amigos <B.S.A@student>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 04:04:38 by Barney e Se       #+#    #+#             */
/*   Updated: 2022/08/16 17:51:01 by Barney e Se      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
# define USER_HPP
# define OPERATOR_PASS "irineu"

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
		std::vector<Channel *>	_channelsVec;
		bool					_auth;
		bool					_oper;

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
		bool					isOper( void );
		void					setOper( void );

};

#endif
