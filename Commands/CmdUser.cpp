#include "CmdUser.hpp"
static bool isAvailable(std::string name, User *user, std::map<int, User*>& fdToUser) {
    if(user->isUsernameValid(name) == true) {
        std::map<int, User*>::iterator it;
        for(it=fdToUser.begin(); it != fdToUser.end(); it++){
            if(name == it->second->getUsername()) {
                return(false);
            }
        }
        return true;
    }
    return false;
}

void CmdUser::execCmd(
    int fd_origin, std::vector<std::string>& cmd,
    const std::string& servername, const std::string& password,
    std::vector<Channel>& allChannels, std::map<int, User*>& fdToUser)
{
    (void) allChannels;
    (void) password;
    User *user = fdToUser[fd_origin];

    if (cmd.size() < 2) {
        sendToUser(fd_origin, "431 * :No username given", 0);
        return;
    }
    std::string correctUser = cmd[1];
    if (isUserValidAuth(*user, 1, 1, 1)) {
        sendToUser(fd_origin, "462 " + user->getUsername() + " :You may not reregister", 0);
    }
    else if (isAvailable(correctUser, user, fdToUser) == false) {  
        sendToUser(fd_origin, "461 * :Not enough parameters", 0);
    }
    else if (user->getIsAuthed() == false) {
        sendToUser(fd_origin,"464 * :Password incorrect", 0);
    }
    else if (isUserValidAuth(*user, 1, 1, 0)) {
        user->setUsername(correctUser);
        sendToUser(fd_origin, "001 " + correctUser + " :Welcome to the IRC Network", 0);
    }
    else {user->setUsername(correctUser);}
}