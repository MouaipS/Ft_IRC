#include "CmdUser.hpp"
#include "Utils.hpp"
#include "Exception.hpp"

CmdUser::CmdUser() : ICommand::ICommand() {};

static bool isAvailable(std::string name, User *user, std::map<int, User*>& fdToUser) {

    if(user->isUsernameValid(name) == true) {

        std::map<int, User*>::iterator it;
        for(it=fdToUser.begin(); it != fdToUser.end(); it++) {

            if (name == it->second->getUsername())
                return false;
        }
        return true;
    }
    return false;
}

void CmdUser::execCmd(
    int fd_origin, std::vector<std::string>& cmd, const std::string& password,
    std::vector<Channel*>& allChannels, std::map<int, User*>& fdToUser)
{
    (void) allChannels;
    (void) password;

    User *user = fdToUser[fd_origin];
    if (cmd.size() < 2)
		throw ExceptionCode(ERR_NONICKNAMEGIVEN);

    std::string correctUser = cmd[1];
    if (isUserValidAuth(*user, 1, 1, 1))
		throw ExceptionCode(ERR_ALREADYREGISTRED);
    else if (isAvailable(correctUser, user, fdToUser) == false)
		throw ExceptionCode(ERR_NEEDMOREPARAMS);
    else if (user->getIsAuthed() == false)
		throw ExceptionCode(ERR_PASSWDMISMATCH);
    else if (isUserValidAuth(*user, 1, 1, 0)) {

        user->setUsername(correctUser);
        serverReply(fd_origin, "001 " + correctUser + " :Welcome to the IRC Network", 0);
    }
    else { user->setUsername(correctUser); }
}
