#include "CmdJoin.hpp"
#include "Exception.hpp"
#include "Utils.hpp"

CmdJoin::CmdJoin() : ICommand::ICommand() {};

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
		throw ExceptionCode(ERR_PASSWDMISMATCH);
	if (cmd.size() == 1)
		throw ExceptionCode(ERR_NEEDMOREPARAMS);
	for (; it != allChannels.end(); it++) {

		if ((*it)->getName() == cmd[1])
		{
			std::cout << "nick: " << user->getNickname() << std::endl;
			if (alreadyOnChannel(user, (*it)->getUsers()))
			{
				std::cout << "User deja dans le channel" << std::endl;
				return ;
			}
			else
			{
				std::cout << "Le Channel existe deja mais le user n'est pas dedans" << std::endl;
				// sendToUser2(fd_origin, user->getNickname() + "!" + user->getUsername() + "@",
							// " JOIN " + cmd[1], 0);
				serverReply(fd_origin, "JOIN " + cmd[1], 0);
				Channel *channel = findChannel(allChannels, cmd[1]);
				channel->setNewUser(user);
				// sendToUser2(fd_origin, user->getNickname() + "!" + user->getUsername() + "@",
				// " 332 = " + cmd[1] + " :" + channel->getTopic(), 0);
				serverReply(fd_origin, "332 = " + cmd[1] + " :" + channel->getTopic(), 0);

				std::string	tmp;
				for (size_t i = 0; i < channel->getUsers().size(); i++) {

					tmp += channel->getUsers()[i]->getNickname();
					if (i == channel->getUsers().size() - 1)
						break;
					tmp += ' ';
				}

				// sendToUser2(fd_origin, user->getNickname() + "!" + user->getUsername() + "@",
				// " 353 = " + cmd[1] + " :" + tmp, 0);
				serverReply(fd_origin, "353 = " + cmd[1] + " :" + tmp, 0);
				return ;
			}
				
		}
	}
	std::cout << "Le channel existe pas donc creation du channel" << std::endl;
	// sendToUser2(fd_origin, user->getNickname() + "!" + user->getUsername() + "@",
				// " JOIN " + cmd[1], 0);
	serverReply(fd_origin, "JOIN " + cmd[1], 0);
	// sendToUser2(fd_origin, user->getNickname() + "!" + user->getUsername() + "@",
			// " 353 = " + cmd[1] + " :" + user->getNickname(), 0);
	serverReply(fd_origin, "353 = " + cmd[1] + " :" + user->getNickname(), 0);
	allChannels.push_back(new Channel(cmd[1]));
	Channel *channel = findChannel(allChannels, cmd[1]);
	channel->setNewUser(user);
	channel->promoteUser(*user);
}
