#include "ICommand.hpp"

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
	message = ":" + _serverName + " " + message + "\r\n";
	send(fd, message.c_str(), message.length(), flag);
}