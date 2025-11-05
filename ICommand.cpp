#include "ICommand.hpp"
#include <stdio.h>
#include <unistd.h>

ICommand::ICommand(std::string serverName) : _serverName(serverName) {};

ICommand::~ICommand() {};

const char* ICommand::UserNotFoundException::what() const throw()
{
	return ("User not found in server list.");
}

bool	ICommand::isUserValidAuth(User& user, bool pass, bool nick, bool username)
{
	if (pass && !user.getIsAuthed())
		return (false);
	if (nick && user.getNickname().empty())
		return (false);
	if (username && user.getUsername().empty())
		return (false);
	return (true);
}

void	ICommand::sendToUser(int fd, std::string message, int flag)
{
	std::string theBuffer;

	std::cout << "DEBUG sendToUser: [" << message << "][" << _serverName << "]" << std::endl;
	theBuffer = ":" + _serverName + " " + message + "\r\n";
	send(fd, theBuffer.c_str(), theBuffer.length(), flag);
	sleep(5);
}

void	ICommand::sendToUser2(int fd, std::string message, std::string message2, int flag)
{
	std::string	theBuffer;

	std::cout << "DEBUG sendToUser2: [" << message << "][" << _serverName << "][" << message2 << "]" << std::endl;
	theBuffer = ":" + message + _serverName + message2 + "\r\n";
	send(fd, theBuffer.c_str(), theBuffer.length(), flag);
	sleep(5);

}
