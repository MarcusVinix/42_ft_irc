/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Barney e Seus Amigos  <B.S.A@students>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 04:04:31 by Barney e Se       #+#    #+#             */
/*   Updated: 2022/08/17 17:39:41 by Barney e Se      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include "ft_irc.hpp"
#include "User.hpp"

class User;

namespace ft
{
	std::vector<std::string>	split( std::string str, char c );

	std::string	joinSplit( std::vector<std::string>::iterator first,
								std::vector<std::string>::iterator last );

	std::string	joinSplit( std::vector<User *> users );

	std::string	toupper( std::string str );

	bool	invalidCharacter( std::string str );

	void	errorMessage( std::string function,	std::string error );
}

#endif
