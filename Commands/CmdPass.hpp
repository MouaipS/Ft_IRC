#pragma once
#include "ICommand.hpp"

#define ERR_NEEDMOREPARAMS "461"
#define ERR_ALREADYREGISTERED "462"
#define ERR_PASSWDMISMATCH "464"

class CmdPass : public ICommand {
public:
	CmdPass(std::string serverName);
    void execCmd(
        int fd_origin,
        std::vector<std::string>& cmd,
        const std::string& name,
        const std::string& password,
        std::vector<Channel>& allChannels,
        std::map<int, User*>& fdToUser
    );
};
