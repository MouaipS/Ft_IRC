#include "CmdJoin.hpp"

CmdJoin::CmdJoin(std::string serverName) : ICommand::ICommand(serverName) {};

static int	alreadyOnChannel(User* user, std::vector<User*> allUsers) {

	for (size_t i = 0; i < allUsers.size(); i++) {

		if (user->getNickname() == allUsers[i]->getNickname())
			return 1;
	}
	return 0;
}

static Channel *findChannel(std::vector<Channel*>& allChannels, std::string name){
	std::vector<Channel*>::iterator it = allChannels.begin();
	for(;it != allChannels.end(); it++) {
		 if((*it)->getName() == name){
			break;
		 }
	}
	return(*it);
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
				std::cout << "User deja dans le channel" << std::endl;
				return ;
			}
			else
			{
				std::cout << "Le Channel existe deja mais le user n'est pas dedans" << std::endl;
				sendToUser2(fd_origin, user->getNickname() + "!" + user->getUsername() + "@",
							" JOIN " + cmd[1], 0);
				Channel *channel = findChannel(allChannels, cmd[1]);
				channel->setNewUser(user);
				return ;
			}
		}
	}
	std::cout << "Le channel existe pas donc creation du channel" << std::endl;
	sendToUser2(fd_origin, user->getNickname() + "!" + user->getUsername() + "@",
				" JOIN " + cmd[1], 0);
	sendToUser(fd_origin, "332 " + user->getNickname() + " " + cmd[1] + " :le topic", 0);
	allChannels.push_back(new Channel(cmd[1]));
	Channel *channel = findChannel(allChannels, cmd[1]);
	channel->setNewUser(user);
	channel->promoteUser(*user);
}
