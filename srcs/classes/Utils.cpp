/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Barney e Seus Amigos <B.S.A@student>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 04:04:34 by Barney e Se       #+#    #+#             */
/*   Updated: 2022/08/16 12:13:19 by Barney e Se      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

Utils::Utils( void ) {
	return ;
}

Utils::~Utils( void ) {
	return ;
}

std::vector<std::string>	Utils::split( std::string str, char c) {

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

std::string	Utils::joinSplit( std::vector<std::string> args ) {

	std::string							msg;
	std::vector<std::string>::iterator	it = args.begin() + 1;

	for ( ; it != args.end(); it++) {
		msg += *it + " ";
	}

	return (msg);

}

std::string	Utils::joinSplit( std::vector<User *> users ) {

	std::string							msg;
	std::vector<User *>::iterator	it = users.begin() + 1;

	for ( ; it != users.end(); it++) {
		msg += (*it)->getNick() + " ";
	}

	return (msg);

}

std::string	Utils::toupper( std::string str ) {

	for (int i = 0; str[i] != '\0'; i++)
		str[i] = std::toupper(str[i]);
	return (str);

}

bool	Utils::invalidCharacter( std::string str ) {

	for (int i = 0; str[i] != '\0'; i++) {
		if (!isalnum(str[i]) && !isalpha(str[i]))
			return (true);
	}
	return (false);

}

void	Utils::errorMessage( std::string function,	std::string error) {

	std::cerr << function << " " << error << std::endl;
	exit(EXIT_FAILURE);

}
