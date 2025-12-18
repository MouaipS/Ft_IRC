#pragma once
#include "ICommand.hpp"

class CmdPrivmsg : public ICommand {
public:
	CmdPrivmsg();
    void execCmd(
        int fd_origin,
        std::vector<std::string>& cmd,
        const std::string& password,
        std::vector<Channel*>& allChannels,
        std::map<int, User*>& fdToUser
    );
};
