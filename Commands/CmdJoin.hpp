#pragma once
#include "ICommand.hpp"

class CmdJoin : public ICommand {
public:
	CmdJoin(std::string serverName);
    void execCmd(
        int fd_origin,
        std::vector<std::string>& cmd,
        const std::string& name,
        const std::string& password,
        std::vector<Channel>& allChannels,
        std::map<int, User*>& fdToUser
    );
};
