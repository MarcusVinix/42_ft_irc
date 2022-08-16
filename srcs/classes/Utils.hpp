/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Barney e Seus Amigos  <B.S.A@students>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 04:04:31 by Barney e Se       #+#    #+#             */
/*   Updated: 2022/08/16 04:04:32 by Barney e Se      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include "ft_irc.hpp"

class Utils {

	public:

		Utils( void );
		~Utils( void );

	public:

		static std::string				toupper( std::string str );
		static std::vector<std::string>	split( std::string str, char c);
		static std::string				joinSplit( std::vector<std::string> args );
		static bool						invalidCharacter( std::string str );

};

#endif
