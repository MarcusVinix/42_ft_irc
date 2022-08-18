/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Barney e Seus Amigos <B.S.A@student>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/16 04:05:01 by Barney e Se       #+#    #+#             */
/*   Updated: 2022/08/18 12:39:00 by Barney e Se      ###   ########.fr       */
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

	this->_args = ft::split(buffer, ' ');
	if (this->_args.size() == 0)
		return ;

	this->_command = ft::toupper(*this->_args.begin());
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
	else if (this->_command == "PASS")
		commandPass();
	else if (this->_command == "QUIT")
		commandQuit();
	else if (this->_command == "HELP")
		commandHelp();
	else if (this->_command == "PONG")
		return ;
	else if (this->_user.isAuth()){
		if (this->_command == "NICK")
			commandNick();
		else if (this->_user.getNick().empty() == false) {
			if (this->_command == "USER")
				commandUser();
			else if (this->_user.getUsername().empty() == false) {
				if (this->_command == "PRIVMSG")
					commandPrivmsg();
				else if (this->_command == "JOIN")
					commandJoin();
				else if (this->_command == "OPER")
					commandOper();
				else if (this->_command == "NOTICE")
					commandNotice();
				else if (this->_command == "PART")
					commandPart();
				else if (this->_command == "WHO")
					commandWho();
				else if (this->_command == "KICK")
					commandKick();
				else if (this->_command == "NAMES")
					commandNames();
				else if (this->_command == "KILL")
					commandKill();
				else if (this->_command == "MODE")
					commandMode();
				else if (this->_command == "DIE")
					commandDie();
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
void	Command::numericResponse( std::string msg, std::string code, int fd, std::string opt ) {

	std::string	response;
	std::string	nick = this->_user.getNick();

	if (nick.empty())
		nick = "Unknown";
	response = ":127.0.0.1 " + code + " " + nick + " ";
	if (opt != "")
		response += opt + " ";
	response += msg + "\r\n";
	if (fd == 0)
		fd = this->_user.getFd();
	if (send(fd, response.c_str(), strlen(response.c_str()), 0) < 0)
		ft::errorMessage("numericResponse: send:", strerror(errno));

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
	if (ft::invalidCharacter(this->_args[0]))
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
	numericResponse("Welcome to the Barney e Seus Amigos ft_irc server " +
		this->_user.getNick() + "! " + this->_user.getUsername() + "@" + this->_user.getHostname(), "001");
	if (this->_ircServer.checkOperators() == false) {
		this->_user.setOper();
		numericResponse("You are worth! Now you became an operator!", "381", this->_user.getFd());
		this->_ircServer.messageToServer(":127.0.0.1 001 all :" + this->_user.getNick() + " is an operator now!", this->_user.getFd());
	}

	return ;

}

void	Command::commandPrivmsg( void ) {

	User		*receive;
	Channel		*channel;
	std::string	msg;
	std::string	response;

	if (this->_args.size() == 0)
		return (numericResponse("A nick must be provide!", "411"));
	if (this->_args.size() == 1)
		return (numericResponse("A message must be provide!", "412"));

	msg = ft::joinSplit(this->_args.begin() + 1, this->_args.end());
	if (this->_args[0][0] != '#') {
		receive = this->_ircServer.getUserByNick(this->_args[0]);
		if (receive == NULL)
			return (numericResponse("Nick not found!", "401"));
		if (msg[0] == ':')
			msg.erase(0, 1);
		response = ":" + this->_user.getNick() + " PRIVMSG " + receive->getNick() + " :" + msg;
		receive->receiveMessage(response);
	} else {
		channel = this->_ircServer.getChannelByName(this->_args[0]);
		if (channel == NULL)
			return (numericResponse("Channel Not Found!", "403"));
		if (channel->getUserByNick(this->_user.getNick()) == NULL)
			return (numericResponse("To send a message to a channel you need to join it!", "442"));
		if (msg[0] == ':')
			msg.erase(0, 1);
		response = ":" + this->_user.getNick() + " PRIVMSG " + channel->getName() + " :" + msg;
		channel->messageToChannel(response, this->_user.getFd());
	}
	return ;

}

void	Command::commandQuit( void ) {

	User		*user;
	std::string	response;

	response = ":" + this->_user.getNick() + " QUIT :" + ft::joinSplit(this->_args.begin() + 1, this->_args.end());

	this->_ircServer.messageAllUsers(response);
	std::cout << BRED << "User left: " << RESET << this->_user.getFd() << std::endl;
	if(this->_user.isOper())
		this->_user.setOper();
	if (this->_ircServer.getUsers().size() != 1	&& this->_ircServer.checkOperators() == false) {
		
		user = *(this->_ircServer.getUsers().begin() + 1);
		if (user != NULL) {
			user->isOper();
			this->_ircServer.messageToServer(":127.0.0.1 001 all :" + user->getNick() + " is an operator now!", user->getFd());
			numericResponse("You are worth! Now you became an operator!", "381", user->getFd());
		}
	}
	this->_ircServer.deleteUser(this->_user.getFd());

	return ;
}

void	Command::commandJoin( void ) {

	Channel		*channel;
	std::string	users;
	std::string	channelName;
	std::string	password;

	if (this->_args[0][0] != '#')
		this->_args[0] = "#" + this->_args[0];
	if (this->_args.size() < 1 || this->_args.size() > 2)
		return (numericResponse("usage: /JOIN <channels> [<keys>]", "461"));
	if (ft::invalidCharacter(this->_args[0].c_str() + 1))
		return (numericResponse("This channel name contain invalid characters!", "403"));

	password = this->_args.size() == 1 ? "" : this->_args[1];
	channel = this->_ircServer.getChannelByName(this->_args[0]);
	if (channel == NULL) {
		channel = new Channel(this->_args[0], password);
		this->_ircServer.addChannel(channel);
	}
	if (password == channel->getPassword())
		this->_user.addChannel(channel);
	else
		return (numericResponse("Password incorrect to channel!", "464"));
	channel->messageFromChannel(":" + this->_user.getNick() + " JOIN " + channel->getName());

	users = ft::joinSplit(channel->getUsers());
	channelName = channel->getName();
	if (channelName[0] == '#')
		channelName.erase(0, 1);
	numericResponse(users, "353", 0, "= " + channelName);
	numericResponse("End of /NAMES list", "366", 0, channelName);

	return ;

}

void	Command::commandOper( void ) {

	User	*user;

	if (this->_args.size() != 2)
		return (numericResponse("usage: /OPER <user> <password>", "461"));
	user = this->_ircServer.getUserByNick(this->_args[0]);
	if (user == NULL)
		return (numericResponse("The user passed doesn't exist!", "401"));
	if (user->isOper())
		return (numericResponse("User is an operator already!", "610"));
	if (this->_args[1] == OPERATOR_PASS)
		user->setOper();
	else
		return (numericResponse("You aren't worth!", "464"));
	this->_ircServer.messageToServer(":127.0.0.1 001 all :" + this->_args[0] + " is an operator now!", user->getFd());
	numericResponse("You are worth! Now you became an operator!", "381", user->getFd());

	return ;

}

void	Command::commandNotice( void ) {

	User		*receiver;
	std::string	notice;
	std::string	response;

	if (this->_args.size() == 0 || this->_args[0].empty()
		|| this->_args.size()  == 1 || this->_args[1].empty())
		return ;
	receiver = this->_ircServer.getUserByNick(this->_args[0]);
	if (receiver == NULL)
		return ;
	notice = ft::joinSplit(this->_args.begin() + 1, this->_args.end());
	if (notice[0] == ':')
		notice.erase(0, 1);
	response = ":" + this->_user.getNick() + " PRIVMSG " + receiver->getNick() + " :" + notice;
	receiver->receiveMessage(response);
	return ;

}

void	Command::commandPart( void ) {

	Channel								*channel;
	std::string							response;
	std::vector<std::string>::iterator	it;

	if (this->_args.size() < 1)
		return (numericResponse("usage: /PART <channel>", "461"));

	it = this->_args.begin();
	for ( ; it != this->_args.end(); it++) {
		if ((*it)[0] == ':')
			it->erase(0, 1);
		if ((*it)[0] != '#')
			*it = '#' + *it;
		channel = this->_ircServer.getChannelByName(*it);
		if (channel == NULL)
			numericResponse("There is no channel with this name!", "403", 0, *it);
		else if (channel->getUserByNick(this->_user.getNick()) == NULL)
			numericResponse("You aren't in this channel!", "442", 0, *it);
		else {
			response = ":" + this->_user.getNick() + " PART " + *it;
			channel->messageFromChannel(response);
			this->_user.removeChannel(channel);
			channel->removeUser(&this->_user);
		}
	}

	return ;

}

/**
 * @brief Command who
 * If no parameter is passed, all users on the server will be send.
 * If one parameter is passed, all users who match the parameter will be send.
 * If two parameter is passed and the second is 'o' all users who match the first parameter
 * and is operator will be send.
 * Every response is a numeric response.
 *
 * @todo adicionar um else if após o if, se o args[0][0] for igual a # quer dizer que é um canal, então o loop será feito com os usuarios do canal.
 *
 */
void	Command::commandWho( void ) {

	std::string						response;
	Channel							*channel;
	std::string						list;
	std::string						channelName;
	std::vector<User *>				users;
	std::vector<User *>::iterator	it;

	if (this->_args.size() > 2)
		return (numericResponse("usage: /WHO [<name> [<o>]]", "461"));

	users = this->_ircServer.getUsers();
	it = users.begin();
	if (this->_args.size() == 0) {
		for ( ; it != users.end(); it++)
			numericResponse((*it)->getRealname(), "352", 0, (*it)->getUsername() + " 0 * " + (*it)->getNick());
	} else {
		if (this->_args[0][0] == '#') {
			channel = this->_ircServer.getChannelByName(this->_args[0]);
			if (channel == NULL)
				return (numericResponse("Channel Not Found!", "403"));
			list = ft::joinSplit(channel->getUsers());
			channelName = channel->getName();
			if (channelName[0] == '#')
				channelName.erase(0, 1);
			numericResponse(list, "352", 0, "Users on " + channelName + ": ");
		} else {
			for ( ; it != users.end(); it++) {
				response = (*it)->getUsername() + " 0 * " + (*it)->getNick();
				if (response.find(this->_args[0], 0) != std::string::npos ||
					(*it)->getRealname().find(this->_args[0], 0) != std::string::npos) {
					if (this->_args.size() == 2 && this->_args[1] == "o" && (*it)->isOper())
						numericResponse((*it)->getRealname(), "352", 0, response);
					else if (this->_args.size() == 1)
						numericResponse((*it)->getRealname(), "352", 0, response);
				}
			}
		}
	}

	return (numericResponse("END of /WHO list", "315"));

}

void	Command::commandKick( void ) {

	Channel		*channel;
	User		*user;
	std::string	msg;
	std::string	response;

	if (this->_args[0] == this->_user.getNick())
		this->_args.erase(this->_args.begin());
	if (this->_args.size() < 2)
		return (numericResponse("usage: /KICK <channel> <user> [<comment>]", "461"));

	if (this->_args[0][0] != '#')
		this->_args[0] = '#' + this->_args[0];
	channel = this->_ircServer.getChannelByName(this->_args[0]);
	if (channel == NULL)
		return (numericResponse("Channel Not Found!", "403", 0, this->_args[0]));

	if (this->_user.isOper() == false)
		return (numericResponse("You need be operator to kick a user!", "482", 0, channel->getName()));
	if (channel->getUserByNick(this->_user.getNick()) == NULL)
		return (numericResponse("You need be a member of the channel to kick a user!", "442", 0, channel->getName()));
	user = channel->getUserByNick(this->_args[1]);
	if (user == NULL)
		return (numericResponse("This user isn't in this channel!", "441", 0, channel->getName() + " " + this->_args[1]));

	msg = ft::joinSplit(this->_args.begin() + 2, this->_args.end());
	if (msg[0] == ':')
		msg.erase(0, 1);
	response = ":" + this->_user.getNick() + " KICK " + channel->getName() + " " + user->getNick() + " " + msg;
	user->removeChannel(channel);
	user->receiveMessage(response);
	channel->removeUser(user);
	channel->messageFromChannel(response);

	return ;

}

void	Command::commandHelp( void ) {

	numericResponse("START of /HELP list", "704");
	numericResponse("Commands Avaliable:", "705", this->_user.getFd());
	numericResponse("PASS, QUIT, HELP, PONG, NICK, USER,", "705", this->_user.getFd());
	numericResponse("PRIVMSG, JOIN, OPER, NOTICE, PART,", "705", this->_user.getFd());
	numericResponse("WHO, KICK, NAMES, KILL, DIE", "705", this->_user.getFd());
	numericResponse("To see how to use a command, send: /<command> ", "705", this->_user.getFd());
	numericResponse("END of /HELP list", "706");

	return ;
}

void	Command::commandNames( void ) {

	Channel								*channel;
	std::vector<Channel *>				listChannel;
	std::string							listUsers;
	std::vector<Channel *>::iterator	it;
	std::vector<std::string>::iterator	it2;

	if (this->_args[0] == this->_user.getNick())
		this->_args.erase(this->_args.begin());
	if (this->_args.size() == 0) {
		listChannel = this->_ircServer.getChannels();
		for ( it = listChannel.begin(); it != listChannel.end(); it++)
			this->_args.push_back((*it)->getName());
	}
	for ( it2 = this->_args.begin(); it2 != this->_args.end(); it2++) {
		channel = this->_ircServer.getChannelByName(*it2);
		if (channel != NULL) {
			listUsers = ft::joinSplit(channel->getUsers());
			if ((*it2)[0] == '#')
				(*it2).erase(0, 1);
			numericResponse(listUsers, "353", 0, "= " + *it2);
			numericResponse("End of /NAMES list", "366", 0, *it2);
		}
	}
	return ;

}

void	Command::commandKill( void ) {

	User		*user;
	std::string	msg;
	std::string	response;

	if (this->_args.size() < 2)
	 	return (numericResponse("usage: /KILL <user> <comment>", "461"));
	if (this->_user.isOper() == false)
		return (numericResponse("You need be operator to kill a user!", "485", this->_user.getFd()));
	user = this->_ircServer.getUserByNick(this->_args[0]);
	if (user == NULL)
		return (numericResponse("The user passed doesn't exist!", "401"));

	msg = ft::joinSplit(this->_args.begin() + 1, this->_args.end());
	if (msg[0] == ':')
	 	msg.erase(0, 1);

	response = ":" + this->_user.getNick() + " KILL " + user->getNick() + " . " + msg;
	user->receiveMessage(response);
	response = user->getNick() + " have been killed by " + this->_user.getNick();
	this->_ircServer.messageToServer(response, user->getFd());
	std::cout << BRED << "User left: " << RESET << user->getFd() << std::endl;
	this->_ircServer.deleteUser(user->getFd());

	return ;

}

void	Command::commandMode( void ) {

	numericResponse("", "324", 0, ft::joinSplit(this->_args.begin(), this->_args.end()));

	return ;
}
void	Command::commandDie( void ) {

	if (this->_args.size() != 1)
		return (numericResponse("usage: /DIE <password>", "461"));
	if (this->_user.isOper() == false)
		return (numericResponse("You need be operator to execute the DIE command!", "482"));
	if (this->_args[0] == DIE_PASS) {
		this->_ircServer.messageAllUsers(this->_user.getNick() + "close the server, thanks for use!");
		loop = false;
	} else {
		return (numericResponse("You aren't worth to close the server!!", "464"));
	}

	return ;
}

