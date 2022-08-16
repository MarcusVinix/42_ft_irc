/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Barney e Seus Amigos <B.S.A@student>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 04:05:06 by Barney e Se       #+#    #+#             */
/*   Updated: 2022/08/16 09:46:59 by Barney e Se      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "ft_irc.hpp"
#include "User.hpp"

class Channel {

	private:

		std::string			_name;
		std::string			_password;
		std::vector<User *>	_users;

	public:

		Channel( std::string name, std::string password );
		~Channel( void );

		std::string			getName( void );
		std::string			getPassword( void );
		std::vector<User *>	getUsers( void );
		User				*getUserByNick( std::string nick );
		void				addUser( User * user );
		void				removeUser( User * user );
		void				messageFromChannel( std::string msg );
		void				messageToChannel( std::string msg, int senderFd );

};

#endif
