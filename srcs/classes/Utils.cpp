/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Barney e Seus Amigos  <B.S.A@students>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 04:04:34 by Barney e Se       #+#    #+#             */
/*   Updated: 2022/08/18 14:22:08 by Barney e Se      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

std::vector<std::string>	ft::split( std::string str, char c) {

	std::string	buff = "";
	std::vector<std::string>	split;

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

}

std::string	ft::joinSplit( std::vector<std::string>::iterator first,
							std::vector<std::string>::iterator last ) {

	std::string	msg;

	for ( ; first != last; first++)
		msg += *first + " ";
	return (msg);

}

std::string	ft::joinSplit( std::vector<User *> users ) {

	std::string						msg;
	std::vector<User *>::iterator	it = users.begin();

	for ( ; it != users.end(); it++)
		msg += (*it)->getNick() + " ";
	return (msg);

}

std::string	ft::toupper( std::string str ) {

	for (int i = 0; str[i] != '\0'; i++)
		str[i] = std::toupper(str[i]);
	return (str);

}

bool	ft::invalidCharacter( std::string str ) {

	for (int i = 0; str[i] != '\0'; i++)
		if (!isalnum(str[i]) && !isalpha(str[i]))
			return (true);
	return (false);

}

void	ft::errorMessage( std::string function,	std::string error ) {

	std::cerr << function << " " << error << std::endl;
	exit(EXIT_FAILURE);

}
