#include "CmdTopic.hpp"
#include "Exception.hpp"
#include "Utils.hpp"

CmdTopic::CmdTopic() : ICommand::ICommand() {};

void CmdTopic::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& password,
    std::vector<Channel*>& allChannels,
    std::map<int, User*>& fdToUser
) {
	(void) password;
	User*							user = fdToUser[fd_origin];

	if (!isUserValidAuth(*user, 1, 1, 1))
		throw ExceptionCode(ERR_PASSWDMISMATCH);
	if (cmd.size() == 1)
		throw ExceptionCode(ERR_NEEDMOREPARAMS);

	for (size_t i = 0; i < allChannels.size(); i++) {

		if (cmd[1] == allChannels[i]->getName()) {

			if (allChannels[i]->findOperator(*user) == -1)
				throw ExceptionCode(ERR_CHANOPRIVSNEEDED);

			if (cmd.size() == 2) {
				if(allChannels[i]->getTopic().empty())
					serverReply(fd_origin, "331 " + user->getNickname() + " " + allChannels[i]->getName() + " :No topic is set",0);
				else
					serverReply(fd_origin, "332 " + user->getNickname() + " " + allChannels[i]->getName() + " :" + allChannels[i]->getTopic(),0);
				return ;
			}
			else
			{
				if (cmd[2].size() == 1 && cmd[2][0] == ':') {

					allChannels[i]->setTopic("");
					std::vector<User*>	myUser = allChannels[i]->getUsers();
					for (size_t i = 0; i < myUser.size(); i++)
						clientReply(myUser[i]->getFd(), user->getNickname(), user->getUsername(), "TOPIC", allChannels[i]->getName(), "", 0);
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
						clientReply(myUser[i]->getFd(), user->getNickname(), user->getUsername(), "TOPIC", cmd[1], message, 0);
					return ;
				}
			}
		}
	}
	throw ExceptionCode(ERR_NOSUCHNICK);
}
