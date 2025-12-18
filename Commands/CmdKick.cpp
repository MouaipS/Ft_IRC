#include "CmdKick.hpp"
#include "Utils.hpp"
#include "Exception.hpp"
CmdKick::CmdKick() : ICommand::ICommand() {};


/**
 * @brief find the target/check the channel/find OP
 * @return flag (1/2/3)
 * - 0 : Empty channel
 * - 1 : Find user in channel 
 * - 2 : Find channel but no user
 * - 3 : Can't find the channel
 * - 4 : Is op
 */

std::vector<Channel*>::iterator	findChannel(std::vector<Channel*>& allChannels, std::string channel) {

    std::vector<Channel*>::iterator it = allChannels.begin();

    for (; it != allChannels.end(); it++) {
        if((*it)->getName() == channel)
			break ;
	}
	return it;
}

int findTarget(std::string target, std::vector<Channel*>::iterator &it, User *&userTarget) {

	if ((*it)->getUsers().empty())
		return(0);

	userTarget = (*it)->findUser(target);
	if (userTarget == NULL)
		return(2);
	if ((*it)->findOperator(*userTarget) != -1)
		return(4);
	return(1);
}

void	sendToUsers(std::vector<Channel*>::iterator &it, User* user, std::string who, std::string reason) {

	std::vector<User*>	allUsers = (*it)->getUsers();

	for (size_t i = 0; i < allUsers.size(); i++) {

        clientReply(allUsers[i]->getFd(), user->getNickname(), user->getUsername(), "KICK " + (*it)->getName(), who, " :" + reason, 0);
	}
}

void CmdKick::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& password,
    std::vector<Channel*>& allChannels,
    std::map<int, User*>& fdToUser
) {
    (void)password;
    std::string reason;

    User *user = fdToUser[fd_origin];
    if (!isUserValidAuth(*user, 1,1,1))
		throw ExceptionCode(ERR_NOTREGISTERED);
    if (cmd.size() == 4)
        reason = cmd[3];
	else if (cmd.size() == 3)
        reason = cmd[2] + " kicked ";
    else
		throw ExceptionCode(ERR_NEEDMOREPARAMS);

    User *target = NULL;
    std::vector<Channel*>::iterator it = findChannel(allChannels, cmd[1]);
	if (it == allChannels.end())
		throw ExceptionCode(ERR_NOSUCHCHANNEL); 

    int checkTarget = findTarget(cmd[2], it, target);
    if (checkTarget == 0)
        serverReply(fd_origin, "353 " + user->getUsername() + " " +cmd[2] + "Empty channel", 0);   
    else if (checkTarget == 3)
		throw ExceptionCode(ERR_NOSUCHCHANNEL); 
    else if (checkTarget == 2)
		throw ExceptionCode(ERR_USERNOTINCHANNEL);
    else {

        User *userTmp;
        int checkOp = findTarget(user->getUsername(), it, userTmp);
        if(checkOp == 2)
			throw ExceptionCode(ERR_NOTONCHANNEL);
        else if(checkOp != 4)
			throw ExceptionCode(ERR_CHANOPRIVSNEEDED);
        else
		{
			sendToUsers(it, user, cmd[2], reason);
			(*it)->removeUserFromChannel(*target);
        }
    }
}
