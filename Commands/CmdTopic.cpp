#include "CmdTopic.hpp"

CmdTopic::CmdTopic(std::string serverName) : ICommand::ICommand(serverName) {};

void CmdTopic::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& password,
    std::vector<Channel*>& allChannels,
    std::map<int, User*>& fdToUser
) {
    // TODO: Implémenter la commande Topic
	(void) password;
	(void) fdToUser;	

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

	for (size_t i = 0; i < allChannels.size(); i++) {

		if (cmd[1] == allChannels[i]->getName()) {

			if (cmd.size() == 2) {

				sendToUser2(fd_origin, user->getNickname() + "!" + user->getUsername(), " TOPIC " + cmd[1] + " :" + allChannels[i]->getTopic(), 0);
				return ;
			}
			else
			{
				if (cmd[2].size() == 1 && cmd[2][0] == ':') {

					allChannels[i]->setTopic("");
					return ;
				}
				else if (cmd[2][0] == ':')
				{
					cmd[2].erase(cmd[2].begin());
					std::string	message;
					for (size_t i = 2; i < cmd.size(); i++) {
						
						message += cmd[i];
						if (i == cmd.size() - 1)
							break ;
						message += ' ';
					}
					allChannels[i]->setTopic(message);

					std::vector<User*>	myUser = allChannels[i]->getUsers();
					for (size_t i = 0; i < myUser.size(); i++)
						sendToUser2(myUser[i]->getFd(), user->getNickname() + "!" + user->getUsername() + "@", " TOPIC " + cmd[1] + " :" + message, 0);
					return ;
				}
			}
		}
	}
	sendToUser(fd_origin, "401 " + cmd[1] + " :No such Nick/Channel", 0);
}
