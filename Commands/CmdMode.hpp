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
    private:
        void ModeO();
        void ModeIp(Channel &channel);
        void ModeIm(Channel &channel);
        void ModeLp(Channel &channel, std::string limit);
        void ModeLm(Channel &channel);
        void ModeKp(Channel &channel, std::string password);
        void ModeKm(Channel &channel, std::string password);
        void ModeT();
};
