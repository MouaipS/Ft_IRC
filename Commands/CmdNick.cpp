#include "CmdNick.hpp"

CmdNick::CmdNick(std::string serverName) : ICommand::ICommand(serverName) {};

static bool isAvailable(std::string CorrectNick, User *user, std::map<int, User*>& fdToUser) {
    if(user->isNicknameValid(CorrectNick) == true) {
        std::map<int, User*>::iterator it;
        for(it=fdToUser.begin(); it != fdToUser.end(); it++){
            if(CorrectNick == it->second->getNickname())
                return(false);
        }
        return true;
    }
    return false;
}

void CmdNick::execCmd(
    int fd_origin, std::vector<std::string>& cmd, const std::string& password,
    std::vector<Channel*>& allChannels,std::map<int, User*>& fdToUser)
{
    (void) allChannels;
    (void) password;
    User *user = fdToUser[fd_origin];

    if(cmd.size() < 2){
        sendToUser(fd_origin, "431 * :No nickname given", 0);
        return;
    }
    std::string CorrectNick = cmd[1];
    if(isAvailable(CorrectNick, user, fdToUser) == false) {  
        sendToUser(fd_origin, "433 * " + CorrectNick + " :Nickname is already in use", 0);
    }
    else if(user->getIsAuthed() == false) {
        sendToUser(fd_origin, "464 " + user->getNickname() + " :Password incorrect", 0);
    }
    else if (isUserValidAuth(*user, 1, 0, 1)) {
        user->setNickname(cmd[1]);
        sendToUser(fd_origin, "001 " + CorrectNick + " :Welcome to the IRC Network", 0);
    }
    else {user->setNickname(cmd[1]);}
}









