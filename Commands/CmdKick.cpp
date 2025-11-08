#include "CmdKick.hpp"

CmdKick::CmdKick(std::string serverName) : ICommand::ICommand(serverName) {};


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
    if(!isUserValidAuth(*user, 1,1,1)){
        sendToUser(fd_origin, "451 " + user->getUsername() + ":You have not registered", 0);
        return;
    }

    if(cmd.size() == 4) {
        reason = cmd[3];
    } else if(cmd.size() == 3) {
        reason = cmd[1] + " kicked ";
    } else {
        sendToUser(fd_origin, "461 " + user->getUsername() + " KICK :Not enough parameters", 0);
        return;
    }

    User *target = NULL;
    int checkTarget = findTarget(cmd[1], cmd[2], allChannels, target);
    if(checkTarget == 0) {
        sendToUser(fd_origin, "353 " + user->getUsername() + " " +cmd[2] + "Empty channel", 0);    
    }else if(checkTarget == 3){
        sendToUser(fd_origin, "403 " + user->getUsername() + " " +cmd[2] + " :No such channel", 0);    
    } else if(checkTarget == 2)
    {
        sendToUser(fd_origin, "441 " + cmd[1] + " " + user->getUsername() + ":They aren't on that channel", 0);
    }
    else{   
        User *userTmp;
        int checkOp = findTarget(user->getUsername(), cmd[2], allChannels, userTmp);
        if(checkOp == 2){
            sendToUser(fd_origin, "442 " + user->getUsername() + " You're not on that channel", 0);
        }
        else if(checkOp != 4){
            sendToUser(fd_origin, "482 " + user->getUsername() + " You're not channel operator", 0);
        }
        else 
        {
            eraseUser(cmd[2], allChannels, (*target));
            sendToUser(fd_origin, "KICK " + cmd[2] + " " + (*target).getNickname() + reason, 0);
        }
    }
}