#include "Command.hpp"

/**
 * @brief Construct a new Command:: Command object
 * Call the function to parser the buffer.
 * 
 * @todo Call the function to check what command is and call the function command.
 * @param buffer Buffer received of the user
 * @param userFd Socket fd from user.
 * @param ircServer Instance of server, with this we can get and modify the user.
 */
Command::Command( std::string buffer, int userFd, IrcServer & ircServer )
	: _user(*ircServer.getUserByFd(userFd)), _ircServer(ircServer) {

	parserBuffer(buffer);
	return;

}

Command::~Command( void ) {
	return ;
}

/**
 * @brief This function will parser the buffer received,
 * removing all spaces and doing a split.
 * The first argument of the buffer will be saved into _command attribute without the '/' .
 * The remaining will be saved into the attribute _args.
 * 
 * @param buffer to be parsed.
 */
void	Command::parserBuffer( std::string buffer ) {
	
	buffer.erase(buffer.end() - 1);
	this->_args = ft_split(buffer, ' ');

	this->_command = ft_toupper(*this->_args.begin());
	if (this->_command[0] == '/')
		this->_command.erase(0, 1);
	this->_args.erase(this->_args.begin());

	// If want to check if its everything okay just run this code.
	// std::vector<std::string>::iterator it;
	// for (it = this->_args.begin(); it != this->_args.end(); it++)
	// 	std::cout << "msg: |" << *it << "|" << std::endl;

	// std::cout << "command: |" << this->_command << "|" << std::endl;

	return ;

}

std::vector<std::string>	Command::ft_split( std::string str, char c) {

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

std::string	Command::ft_toupper( std::string str ) {

	for (int i = 0; str[i] != '\0'; i++)
		str[i] = std::toupper(str[i]);
	return (str);

}