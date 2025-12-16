#include "Utils.hpp"
#include <sys/socket.h>
#include <ctime>

void	serverReply(int fd, std::string message, int flag)
{
	std::string theBuffer;

	theBuffer = ":" + getServerName() + " " + message + "\r\n";
	send(fd, theBuffer.c_str(), theBuffer.length(), flag);
	usleep(10);
}

void	clientReply(int fd, std::string nick, std::string user, std::string command, std::string target, std::string message, int flag)
{
	std::string theBuffer;

	theBuffer = ":" + nick + "!" + user + "@" + getServerName() + " " + command + " " + target + " :" + message + "\r\n";
	send(fd, theBuffer.c_str(), theBuffer.length(), flag);
	usleep(10);
}

void	noticeReply(Channel *channel, std::string message){
	std::string theBuffer;

	std::vector<User*> users = channel->getUsers();
	std::vector<User*>::iterator it = users.begin();
	for(; it != channel->getUsers().end(); it++){
		theBuffer = ":" + getServerName() + " NOTICE " + (*it)->getNickname() + " :" + message + "\r\n";
		send((*it)->getFd(), message.c_str(), message.length(), 0);
	}
}

void	noticeReply(User *user, std::string message){
	std::string theBuffer;

	theBuffer = ":" + getServerName() + " NOTICE " + user->getNickname() + " :" + message + "\r\n";
	send(user->getFd(), message.c_str(), message.length(), 0);
}


bool	isUserValidAuth(User& user, bool pass, bool nick, bool username)
{
	if (pass && !user.getIsAuthed())
		return (false);
	if (nick && user.getNickname().empty())
		return (false);
	if (username && user.getUsername().empty())
		return (false);
	return (true);
}

std::string	displayTimestamp(void) {
	
	time_t 		stime;
	struct tm	*timeinfos;
	char		buff[25];

	time (&stime);
	timeinfos = localtime (&stime);
	std::strftime(buff, 25, "%c", timeinfos);
	return buff;
}

std::string	getServerName() { return "irCnews"; }
