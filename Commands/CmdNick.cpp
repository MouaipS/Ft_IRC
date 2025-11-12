#include "CmdNick.hpp"
#include "Exception.hpp"
#include "Utils.hpp"

CmdNick::CmdNick() : ICommand::ICommand() {};

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

	std::cout << "In CmdNick::execCmd" << std::endl;
    if(cmd.size() < 2)
		throw ExceptionCode(ERR_NONICKNAMEGIVEN);
    std::string CorrectNick = cmd[1];
    if (isAvailable(CorrectNick, user, fdToUser) == false)
		throw ExceptionCode(ERR_NICKNAMEINUSE);
    else if (user->getIsAuthed() == false)
		throw ExceptionCode(ERR_PASSWDMISMATCH);
    else if (isUserValidAuth(*user, 1, 0, 1)) {
        user->setNickname(cmd[1]);
		user->setFd(fd_origin);
        serverReply(fd_origin, "001 " + CorrectNick + " :Welcome to the IRC Network", 0);
    }
    else {user->setNickname(cmd[1]);}
}









