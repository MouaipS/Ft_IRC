#pragma once
#include "ICommand.hpp"

class CmdMode : public ICommand {
    public:
	    CmdMode();
        void execCmd(
            int fd_origin,
            std::vector<std::string>& cmd,
            const std::string& password,
            std::vector<Channel*>& allChannels,
            std::map<int, User*>& fdToUser
        );
    private:
        void ModeOp(Channel &channel, std::string target);
        void ModeIp(Channel &channel);
        void ModeOm(Channel &channel, std::string target);
        void ModeIm(Channel &channel);
        void ModeLp(Channel &channel, std::string limit);
        void ModeLm(Channel &channel);
        void ModeKp(Channel &channel, std::vector<std::string>& cmd);
        void ModeKm(Channel &channel);
        void ModeTp(Channel &channel);
        void ModeTm(Channel &channel);
        void sendUsers(std::vector<User *> users, Channel *channel, std::string mode);
};
