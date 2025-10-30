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
	//TODO verifier que user est authentifier + que cmd[1] existe
	std::vector<Channel*>::iterator	it = allChannels.begin();
	User*							user = fdToUser[fd_origin];

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
				sendToUser2(fd_origin, user->getNickname() + "!" + user->getUsername() 					+ "@", "JOIN " + cmd[1] + " * :realname", 0); 
				allChannels[0]->setNewUser(user);
				return ;
			}
		}
	}
	std::cout << "creer channel" << std::endl;
	sendToUser2(fd_origin, user->getNickname() + "!" 
	+ user->getUsername() + "@", "JOIN " + cmd[1] + " * :realname", 0); 
	allChannels.push_back(new Channel(cmd[1]));
	allChannels[0]->setNewUser(user);
}
