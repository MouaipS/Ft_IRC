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
int findTarget(std::string target, std::string channel, std::vector<Channel*>& allChannels, User *&userTarget) {
    std::vector<Channel*>::iterator it = allChannels.begin();
    for(; it != allChannels.end(); it++){
        if((*it)->getName() == channel)
        {
            if((*it)->getUsers().empty()) return(0);
            userTarget = (*it)->findUser(target);
            if(userTarget == NULL)
                return(2);
            if((*it)->findOperator(*userTarget) != -1)
                return(4);
            return(1);
        }
    }
    return(3);
}

void eraseUser(std::string channel, std::vector<Channel*>& allChannels, User &userTarget){
    std::vector<Channel*>::iterator it = allChannels.begin();
    for(; it != allChannels.end(); it++){
        if((*it)->getName() == channel){
            (*it)->removeUserFromChannel(userTarget);
            return;
        }
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
    if(!isUserValidAuth(*user, 1,1,1))
		throw ExceptionCode(ERR_NOTREGISTERED);
    if(cmd.size() == 4) {
        reason = cmd[3];
    } else if(cmd.size() == 3) {
        reason = cmd[1] + " kicked ";
    } else
		throw ExceptionCode(ERR_NEEDMOREPARAMS);
    User *target = NULL;
    int checkTarget = findTarget(cmd[1], cmd[2], allChannels, target);
    if (checkTarget == 0)
        serverReply(fd_origin, "353 " + user->getUsername() + " " +cmd[2] + "Empty channel", 0);   
    else if(checkTarget == 3)
		throw ExceptionCode(ERR_NOSUCHCHANNEL); 
    else if(checkTarget == 2)
		throw ExceptionCode(ERR_USERNOTINCHANNEL);
    else {
        User *userTmp;
        int checkOp = findTarget(user->getUsername(), cmd[2], allChannels, userTmp);
        if(checkOp == 2)
			throw ExceptionCode(ERR_NOTONCHANNEL);
        else if(checkOp != 4)
			throw ExceptionCode(ERR_CHANOPRIVSNEEDED);
        else {
            eraseUser(cmd[2], allChannels, (*target));
            serverReply(fd_origin, "KICK " + cmd[2] + " " + (*target).getNickname() + " :" + reason, 0);
        }
    }
}
