#include "CmdTopic.hpp"

CmdTopic::CmdTopic(std::string serverName) : ICommand::ICommand(serverName) {};

void CmdTopic::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& password,
    std::vector<Channel*>& allChannels,
    std::map<int, User*>& fdToUser
) {
    // TODO: Implémenter la commande Topic
	(void) fd_origin;
	(void) cmd;
	(void) password;
	(void) allChannels;
	(void) fdToUser;	
}
