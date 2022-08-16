/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Barney e Seus Amigos  <B.S.A@students>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 04:05:01 by Barney e Se       #+#    #+#             */
/*   Updated: 2022/08/16 04:13:02 by Barney e Se      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

/**
 * @brief Construct a new Command:: Command object
 * Call the function to parser the buffer.
 *
 * @param buffer Buffer received of the user
 * @param userFd Socket fd from user.
 * @param ircServer Instance of server, with this we can get and modify the user.
 */
Command::Command( std::string buffer, int userFd, IrcServer & ircServer )
	: _user(*ircServer.getUserByFd(userFd)), _ircServer(ircServer) {

	parserBuffer(buffer);
	checkCommand();
	return ;

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

	if (buffer[0] == '/')
		buffer.erase(0, 1);
	buffer.erase(std::remove(buffer.begin(), buffer.end(), '\n'), buffer.end());
	buffer.erase(std::remove(buffer.begin(), buffer.end(), '\r'), buffer.end());

	this->_args = Utils::split(buffer, ' ');
	if (this->_args.size() == 0)
		return ;

	this->_command = Utils::toupper(*this->_args.begin());
	this->_args.erase(this->_args.begin());
	if (this->_args[0][0] == ':')
		this->_args[0].erase(0, 1);

	return ;

}

/**
 * @brief Function to check the command passed.
 * This function will check what command is and
 * call the right function to execute it.
 * If some error is found a numeric response will be send.
 *
 * about commands:
 * https://simple.wikipedia.org/wiki/List_of_Internet_Relay_Chat_commands
 *
 * about numericResponse:
 * https://www.alien.net.au/irc/irc2numerics.html
 */
void	Command::checkCommand( void ) {

	if (this->_command == "CAP")
		return (numericResponse("CAP * ACK multi-prefix", ""));
	if (this->_command == "PASS")
		commandPass();
	else if (this->_user.isAuth()){
		if (this->_command == "NICK")
			commandNick();
		else if (this->_user.getNick().empty() == false) {
			if (this->_command == "USER")
				commandUser();
			else if (this->_user.getUsername().empty() == false) {
				if (this->_command == "PRIVMSG")
					commandPrivmsg();
			}
			else
				return (numericResponse("A user must be provide: usage: /USER <username> <hostname> <servername> <realname>", "431"));
		}
		else
			return (numericResponse("A nick must be provide: usage: /NICK <nick>", "431"));
	} else
		return (numericResponse("Password was required: usage: /PASS <password>", "464"));

	return ;

}

/**
 * @brief Function to send numeric response of success and fail to the user.
 *
 * @param msg To be send.
 * @param code of status.
 */
void	Command::numericResponse( std::string msg, std::string code ) {

	std::string	response;
	std::string	nick = this->_user.getNick();

	if (nick.empty())
		nick = "Unknown";
	response = nick + ": " + code + ": " + msg + "\r\n";

	if (send(this->_user.getFd(), response.c_str(), strlen(response.c_str()), 0) < 0) {
		std::cerr << "numericResponse: send: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
	return ;

}

/**
 * @brief Function to use the command Pass.
 * This command will authenticate the user.
 * After that the user can use the server normally.
 *
 */
void	Command::commandPass( void ) {

	if (this->_user.isAuth())
		return (numericResponse("User already registered!", "462"));
	if (this->_args.empty() || this->_args.size() != 1)
		return (numericResponse("usage: /PASS <password>", "461"));
	if (this->_args[0] == this->_ircServer.getPassword()) {
		this->_user.auth();
		return (numericResponse("Password Correct!", "338"));
	} else
		return (numericResponse("Incorrect Password!", "339"));

}

/**
 * @brief Function to use the command Nick.
 * This command will get the nick of the user.
 * With success the nick will be changed and
 * all the users of the server will receive a notification about the change.
 * On fail a numeric response will be send.
 *
 */
void	Command::commandNick( void ) {

	std::vector<User *>			usersVec;
	std::vector<User *>::iterator	it;

	if (this->_args.size() != 1)
		return (numericResponse("usage: /NICK <nick>", "431"));
	if (this->_args[0].empty())
		return (numericResponse("Nick cannot be empty!", "432"));
	if (this->_args[0] == "anonymous")
		return (numericResponse("This nick can't be used!", "432"));
	if (Utils::invalidCharacter(this->_args[0]))
		return (numericResponse("This nick contain invalids characters!", "432"));

	usersVec = this->_ircServer.getUsers();
	it = usersVec.begin();
	for( ; it != usersVec.end(); it++) {
		if (this->_args[0] == (*it)->getNick())
			return (numericResponse("This nick is already in use!", "433"));
	}
	this->_ircServer.messageAllUsers(this->_user.getNick() + ": change nick to " + this->_args[0]);
	this->_user.setNick(this->_args[0]);

	return ;

}

/**
 * @brief Function to use the command user.
 * This command will save a lot of informations about the user and send welcome.
 *
 * @todo Maybe we do some validation to the information.
 *
 */
void	Command::commandUser( void ) {

	if (this->_args.size() != 4)
		return (numericResponse("usage: /USER <username> <hostname> <servername> <realname>", "461"));
	if (this->_user.getUsername().empty() == false)
		return (numericResponse("You are already register!", "462"));
	this->_user.setUsername(this->_args[0]);
	this->_user.setHostname(this->_args[1]);
	this->_user.setServername(this->_args[2]);
	this->_user.setRealname(this->_args[3]);
	numericResponse("Welcome to the Barney e Seus Amigos ft_irc server" +
		this->_user.getNick() + "!" + this->_user.getUsername() + "@" + this->_user.getHostname(), "001");
	return ;

}

void	Command::commandPrivmsg( void ) {

	User		*receive;
	std::string	msg;
	std::string	response;

	if (this->_args.size() == 0)
		return (numericResponse("A nick must be provide!", "411"));
	if (this->_args.size() == 1)
		return (numericResponse("A message must be provide!", "412"));

	receive = this->_ircServer.getUserByNick(this->_args[0]);
	if (receive == NULL)
		return (numericResponse("Nick not found!", "401"));
	msg = Utils::joinSplit(this->_args);
	response = ":" + this->_user.getNick() + " PRIVMSG " + receive->getNick() + " :" + msg;
	receive->receiveMessage(response);
	return ;

}
//:USER COMAND :MSG