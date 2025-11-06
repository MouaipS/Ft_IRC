#include "CmdJoin.hpp"

CmdJoin::CmdJoin(std::string serverName) : ICommand::ICommand(serverName) {};

static int	alreadyOnChannel(User* user, std::vector<User*> allUsers) {

	for (size_t i = 0; i < allUsers.size(); i++) {

		if (user->getNickname() == allUsers[i]->getNickname())
			return 1;
	}
	return 0;
}

void CmdJoin::execCmd(
		int fd_origin,
		std::vector<std::string>& cmd,
		const std::string& password,
		std::vector<Channel*>& allChannels, 
		std::map<int, User*>& fdToUser )
{

	(void) password;
	//TODO verifier que user est authentifié + que cmd[1] existe
	std::vector<Channel*>::iterator	it = allChannels.begin();
	User*							user = fdToUser[fd_origin];

	if (!isUserValidAuth(*user, 1, 1, 1))
	{
		sendToUser(fd_origin, "464 :Password incorrect", 0);
		return ;
	}
	if (cmd.size() == 1)
	{
		sendToUser(fd_origin, "461 :Not enough parameters", 0);
		return ;
	}
	for (; it != allChannels.end(); it++) {

		if ((*it)->getName() == cmd[1])
			{
			if (alreadyOnChannel(user, (*it)->getUsers()))
			{
				std::cout << "deja la" << std::endl;
				return ;
			}
			else
			{
				std::cout << "channel existe deja" << std::endl;
				sendToUser2(fd_origin, user->getNickname() + "!" + user->getUsername() + "@",
							" JOIN " + cmd[1], 0);
				allChannels[0]->setNewUser(user);
				return ;
			}
		}
	}
	std::cout << "creer channel" << std::endl;
	sendToUser2(fd_origin, user->getNickname() + "!" + user->getUsername() + "@",
				" JOIN " + cmd[1], 0);
	sendToUser(fd_origin, "332 " + user->getNickname() + " " + cmd[1] + " :le topic", 0);
	allChannels.push_back(new Channel(cmd[1]));
	allChannels[0]->setNewUser(user);
<<<<<<< Updated upstream
	allChannels[0]->promoteUser(*user);
=======
	allChannels[0]->promoteUser(user);
	sendToUser(fd_origin, "353 " + user->getNickname() + " = " + cmd[1] + " :@" 
				+ user->getNickname(), 0);
	// sendToUser(fd_origin, "366 " + user->getNickname() + " " + cmd[1] + " :End of /NAMES list.", 0);
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
}
