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
	if (this->_args.size() == 0)
		return ;

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

/**
 * @brief Function to check the command passed.
 * This function will check what command is and
 * call the right function to execute it.
 * If some error is found a numeric response will be send.
 */
void	Command::checkCommand( void ) {

	if (this->_command == "PASS")
		commandPass();
	else if (this->_user.isAuth()){
		if (this->_command == "NICK")
			commandNick();
		else if (this->_user.getNick().empty() == false) {
			if (this->_command == "USER")
				commandUser();
			else if (this->_user.getUsername().empty() == false) {

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
	if (invalidCharacter(this->_args[0]))
		return (numericResponse("This nick contain invalids characters!", "432"));

	usersVec = this->_ircServer.getUsers();
	it = usersVec.begin();
	for( ; it != usersVec.end(); it++) {
		if (this->_args[0] == (*it)->getNick())
			return (numericResponse("This nick is already in use!", "433"));
	}
	this->_ircServer.messageAllUsers(this->_user.getNick() + ": change nick to " + this->_args[0] + "\n");
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

	if (this->_args.size() < 4)
		return (numericResponse("usage: /USER <username> <hostname> <servername> <realname>", "461"));
	if (this->_user.getUsername().empty() == false)
		return (numericResponse("You are already register!", "462"));
	this->_user.setUsername(this->_args[0]);
	this->_user.setHostname(this->_args[1]);
	this->_user.setServername(this->_args[2]);
	this->_user.setRealname(this->_args[3]);
	numericResponse("Welcome to the Barney e seus amigos ft_irc server" +
		this->_user.getNick() + "!" + this->_user.getUsername() + "@" + this->_user.getHostname(), "001");
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
	int			exitCode;

	if (nick.empty())
		nick = "Unknown";
	response = nick + ": " + code + ": " + msg + "\n";
	exitCode = send(this->_user.getFd(), response.c_str(), strlen(response.c_str()), 0);
	if (exitCode < 0) {
		std::cerr << "numericResponse: send: " << strerror(errno) << std::endl;
		exit(EXIT_FAILURE);
	}
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

bool	Command::invalidCharacter( std::string str ) {

	for (int i = 0; str[i] != '\0'; i++) {
		if (!isalnum(str[i]) && !isalpha(str[i]))
			return (true);
	}
	return (false);

}
