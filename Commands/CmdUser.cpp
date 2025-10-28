#include "CmdUser.hpp"

void CmdUser::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& name,
    const std::string& password,
    std::vector<Channel>& allChannels,
    std::map<int, User*>& fdToUser
) {
    // TODO: Implémenter la commande User
    (void) allChannels;
    (void) password;
    if (ICommand::isUserValidAuth(*fdToUser[fd_origin], 0, 0, 0)) {
        fdToUser[fd_origin]->setUsername(cmd[1]);
        std::string message = ":";
        message = message + name + " 001 " + fdToUser[fd_origin]->getUsername() + " :Welcome to the IRC Network " + fdToUser[fd_origin]->getUsername() + "\r\n";  //001 RPL_WELCOME
        ICommand::sendToUser(fd_origin, message, 0);
    }
}
