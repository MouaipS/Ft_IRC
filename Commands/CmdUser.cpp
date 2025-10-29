#include "CmdUser.hpp"
bool isAvailable(std::string name, User *user, std::map<int, User*>& fdToUser) {
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
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& servername,
    const std::string& password,
    std::vector<Channel>& allChannels,
    std::map<int, User*>& fdToUser
) {
    (void) allChannels;
    (void) password;
    if(isAvailable(cmd[1], fdToUser[fd_origin], fdToUser) == false) {  
        //Si Username not available ou pas isalphanum : 461 ERR_NEEDMOREPARAMS "<client> <command> :Not enough parameters" 
        std::string message = ":";
        message = message + servername + " 461 " + fdToUser[fd_origin]->getUsername() + " :Not enough parameters" + fdToUser[fd_origin]->getUsername() + "\r\n";
        std::cout << "test 1" << std::endl;
        ICommand::sendToUser(fd_origin, message, 0);
    }
    else if (ICommand::isUserValidAuth(*fdToUser[fd_origin], 1, 1, 0)) {
        //PASS 1 NICK 1 USER 0 -> Code "001" MSG ":Welcome to the IRC Network"
        fdToUser[fd_origin]->setUsername(cmd[1]);
        std::string message = ":";
        message = message + servername + " 001 " + servername + " :Welcome to the IRC Network " + fdToUser[fd_origin]->getUsername() + "\r\n";  //001 RPL_WELCOME
        std::cout << "test 2" << std::endl;
        ICommand::sendToUser(fd_origin, message, 0);
    }
    else if(fdToUser[fd_origin]->getIsAuthed() == false) {
        //PASS 0 NICK 0 USER 0 -> Code "464" MSG "ERR_PASSWDMISMATCH"
        //PASS 0 NICK 1 USER 0 -> Code "464" MSG "ERR_PASSWDMISMATCH"
        //PASS 0 NICK 0 USER 1 -> Code "464" MSG "ERR_PASSWDMISMATCH"
        //PASS 0 NICK 1 USER 1 -> Code "464" MSG "ERR_PASSWDMISMATCH"
        std::string message = ":";
        message = message + servername + " 464 " + fdToUser[fd_origin]->getUsername() + " :NERR_PASSWDMISMATCH" + fdToUser[fd_origin]->getUsername() + "\r\n";
        std::cout << "test 3" << std::endl;
        ICommand::sendToUser(fd_origin, message, 0);
    }
    else if(ICommand::isUserValidAuth(*fdToUser[fd_origin], 1, 0, 1) || ICommand::isUserValidAuth(*fdToUser[fd_origin], 1, 1, 1)){
        //PASS 1 NICK 0 USER 1 -> Code "462" MSG "ALREADY REGISTERED"
        //PASS 1 NICK 1 USER 1 -> Code "462" MSG "ALREADY REGISTERED" 
        std::string message = ":";
        message = message + servername + " 462 " + fdToUser[fd_origin]->getUsername() + " :ALREADY REGISTERED" + fdToUser[fd_origin]->getUsername() + "\r\n";
        ICommand::sendToUser(fd_origin, message, 0);
        std::cout << "test 4" << std::endl;
    }
    else {
        //PASS 1 NICK 0 USER 0 -> RIEN (ON CHANGE SIMPLEMENT USERNAME)
        fdToUser[fd_origin]->setUsername(cmd[1]);
        std::cout << "test 5" << std::endl;
    }
}









