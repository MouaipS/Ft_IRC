#include "CmdPrivmsg.hpp"

CmdPrivmsg::CmdPrivmsg(std::string serverName) : ICommand::ICommand(serverName) {};

void CmdPrivmsg::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& password,
    std::vector<Channel*>& allChannels,
    std::map<int, User*>& fdToUser
) {

	User*							user = fdToUser[fd_origin];
	if (!isUserValidAuth(*user, 1, 1, 1))
	{
		sendToUser(fd_origin, "464 :Password incorrect", 0);
		return ;
	}
	for (size_t i = 0; i <)
    // TODO: Implémenter la commande Privmsg
	(void) fd_origin;
	(void) cmd;
	(void) password;
	(void) allChannels;
	(void) fdToUser;	
}
