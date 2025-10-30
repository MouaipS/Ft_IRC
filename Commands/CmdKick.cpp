#include "CmdKick.hpp"

CmdKick::CmdKick(std::string serverName) : ICommand::ICommand(serverName) {};

void CmdKick::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& password,
    std::vector<Channel*>& allChannels,
    std::map<int, User*>& fdToUser
) {
    // TODO: Implémenter la commande Kick
	(void) fd_origin;
	(void) cmd;
	(void) password;
	(void) allChannels;
	(void) fdToUser;	
}
