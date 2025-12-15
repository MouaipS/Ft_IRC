#include "CmdNick.hpp"
#include "Exception.hpp"
#include "Utils.hpp"

CmdNick::CmdNick(std::string time) : ICommand::ICommand(), _timeOfDay(time) {};

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

    if(cmd.size() < 2)
		throw ExceptionCode(ERR_NONICKNAMEGIVEN);
    std::string CorrectNick = cmd[1];
    if (isAvailable(CorrectNick, user, fdToUser) == false)
		throw ExceptionCode(ERR_NICKNAMEINUSE);
    else if (user->getIsAuthed() == false)
		throw ExceptionCode(ERR_PASSWDMISMATCH);
    else if(isUserValidAuth(*user, 1, 1, 1))
        user->setNickname(cmd[1]);
    else if (isUserValidAuth(*user, 1, 0, 1)) {
        user->setNickname(cmd[1]);
		user->setFd(fd_origin);
        serverReply(fd_origin, "001 " + CorrectNick + " :Welcome to the IRC Network", 0);
        serverReply(fd_origin, "002 " + CorrectNick + " :Your host is " + getServerName() + " running version InspIRCd v4", 0);
        serverReply(fd_origin, "003 " + CorrectNick + " :This server was created " + _timeOfDay, 0);
        serverReply(fd_origin, "004 " + CorrectNick + " :" + getServerName() + " InspIRCD v4 itkol", 0);
    }
    else {user->setNickname(cmd[1]);}
}









