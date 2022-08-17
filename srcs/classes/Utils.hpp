/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Barney e Seus Amigos <B.S.A@student>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 04:04:31 by Barney e Se       #+#    #+#             */
/*   Updated: 2022/08/17 09:30:37 by Barney e Se      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include "ft_irc.hpp"
#include "User.hpp"


template < class T = std::string >
class Utils {

	class User;

	public:

		Utils( void ) { return ; };
		~Utils( void ) { return ; };

		typedef typename std::vector<T>::iterator	Iterator;

		static T				toupper( T str ) {

			for (int i = 0; str[i] != '\0'; i++)
					str[i] = std::toupper(str[i]);
			return (str);

		};

		static std::vector<T>	split( T str, char c) {

			T	buff = "";
			std::vector<T>	split;

			for (int i = 0; str[i] != '\0'; i++) {
				if (str[i] != c)
					buff += str[i];
				else if (str[i] == c && buff != "") {
					split.push_back(buff);
					buff = "";
				}
			}
			if (buff != "")
				split.push_back(buff);
			return (split);

		};

		static T				joinSplit( Iterator first, Iterator last ) {

			T	msg;

			for ( ; first != last; first++) {
				msg += *first + " ";
			}

			return (msg);

		};



		static bool				invalidCharacter( T str ) {

			for (int i = 0; str[i] != '\0'; i++) {
				if (!isalnum(str[i]) && !isalpha(str[i]))
					return (true);
			}
			return (false);

		};

		static void				errorMessage( T function, T error) {

			std::cerr << function << " " << error << std::endl;
			exit(EXIT_FAILURE);

		};

};



#endif
