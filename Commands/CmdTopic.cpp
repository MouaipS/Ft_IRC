#include "CmdTopic.hpp"

void CmdTopic::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& name,
    const std::string& password,
    std::vector<Channel>& allChannels,
    std::map<int, User*>& fdToUser
) {
    // TODO: Implémenter la commande Topic
	(void) fd_origin;
	(void) cmd;
	(void) name;
	(void) password;
	(void) allChannels;
	(void) fdToUser;	
}
