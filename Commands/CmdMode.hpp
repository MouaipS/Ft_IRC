#pragma once
#include "ICommand.hpp"

class CmdMode : public ICommand {
public:
	CmdMode(std::string serverName);
    void execCmd(
        int fd_origin,
        std::vector<std::string>& cmd,
        const std::string& password,
        std::vector<Channel*>& allChannels,
        std::map<int, User*>& fdToUser
    );
};
