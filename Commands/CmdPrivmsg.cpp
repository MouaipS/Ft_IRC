#include "CmdPrivmsg.hpp"

CmdPrivmsg::CmdPrivmsg(std::string serverName) : ICommand::ICommand(serverName) {};

void CmdPrivmsg::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& password,
    std::vector<Channel*>& allChannels,
    std::map<int, User*>& fdToUser
) {
	(void) password;
	User*	user = fdToUser[fd_origin];
	if (!isUserValidAuth(*user, 1, 1, 1))
	{
		sendToUser(fd_origin, "464 :Password incorrect", 0);
		return ;
	}
	
	std::string mail;
	for (size_t i = 2; i < cmd.size(); i++)
	{
		mail += cmd[i];
		if (i == cmd.size() - 1)
		{
			mail += "\n\f";
			break ;
		}
		mail += ' ';
	}
	for (size_t i = 0; i < allChannels.size(); i++) {
		
		if (cmd[1] == allChannels[i]->getName()) {
			
			std::vector<User*>	myUser = allChannels[i]->getUsers();
			for (size_t j = 0; j < myUser.size(); j++)
				send(myUser[j]->getFd(), mail.c_str(), mail.length(), 0);
				// sendToUser(myUser[j]->getFd(), cmd[2], 0);
			return ;
		}
	}

	std::map<int, User*>::iterator	it = fdToUser.begin();
	for (; it != fdToUser.end(); it++) {

		if (it->second->getUsername() == cmd[1])
		{
			send(it->second->getFd(), mail.c_str(), mail.length(), 0);
			// sendToUser(it->second->getFd(), cmd[2], 0);
			return ;
		}
	}
}
