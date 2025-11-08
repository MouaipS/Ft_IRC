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

	// checking the manda security (authorisation and command requested size)
	if (!isUserValidAuth(*user, 1, 1, 1)) {
		sendToUser(fd_origin, "464 :Password incorrect", 0);
		return ;
	}
	if (cmd.size() < 3) {
		sendToUser(fd_origin, "412 :No text to send", 0);
		return ;
	}

	//writing the full message to send
	std::string mail;
	for (size_t i = 2; i < cmd.size(); i++)
	{
		mail += cmd[i];
		if (i == cmd.size() - 1)
			break ;
		mail += ' ';
	}

	// because you can't say if the target is a channel or a nickname,
	// I start by checking the channels first
	for (size_t i = 0; i < allChannels.size(); i++) {
		
		if (cmd[1] == allChannels[i]->getName()) {
			
			// channel good but check if user is on the channel
			if (allChannels[i]->findUser(user->getUsername()))
			{
				// send the message to all of the users on the channel
				std::vector<User*>	myUser = allChannels[i]->getUsers();
				for (size_t j = 0; j < myUser.size(); j++)
					sendToUser2(myUser[j]->getFd(), user->getNickname() + "!" + user->getUsername() + "@", " PRIVMSG " + cmd[1] + " :" + mail, 0);
				return ;
			}
			// otherwise he can't send the message
			else
			{
				sendToUser(fd_origin, "405 " + cmd[1] + " :Cannot send to channel", 0);
				return ;
			}
		}
	}

	// if the target is not a channel, it must be a nickname
	std::map<int, User*>::iterator	it = fdToUser.begin();
	for (; it != fdToUser.end(); it++) {

		if (it->second->getNickname() == cmd[1])
		{
			// send the message to the user
			sendToUser2(it->second->getFd(), user->getNickname() + "!" + user->getUsername() + "@", " PRIVMSG " + cmd[1] + " :" + mail, 0);
			return ;
		}
	}

	// the target don't exist
	sendToUser(fd_origin, "401 " + cmd[1] + " :No such Nick/Channel", 0);
}
