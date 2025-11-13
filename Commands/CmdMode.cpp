#include "CmdMode.hpp"
#include "Exception.hpp"
#include "Utils.hpp"

CmdMode::CmdMode() : ICommand::ICommand() {};

static Channel *findChannel(std::string channel, std::vector<Channel*>& allChannels) {

    std::vector<Channel*>::iterator it = allChannels.begin();
    for(; it != allChannels.end(); it++){
        if((*it)->getName() == channel)
            return((*it));
    }
	return 0;
}

void CmdMode::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& password,
    std::vector<Channel*>& allChannels,
    std::map<int, User*>& fdToUser
) {

    Channel *channel;
    User *user = fdToUser[fd_origin];
    if (!isUserValidAuth(*user, 1,1,1))
		throw ExceptionCode(ERR_NOTREGISTERED);
    if (cmd.size() < 3 || cmd[2].empty())
		throw ExceptionCode(ERR_NEEDMOREPARAMS);
    channel = findChannel(cmd[2], allChannels);
	if (!channel)
		throw ExceptionCode(ERR_NOSUCHCHANNEL);
    if (channel->findUser(*user) == -1) {
		throw ExceptionCode(ERR_NOTONCHANNEL);
    } else if (channel->findOperator(*user) == -1) {
		throw ExceptionCode(ERR_CHANOPRIVSNEEDED);
    }

    const std::string level[10] = {"+i","-i","+k","-k","+l","-l","+o","-o","+t","-t"};
	int selectLevel = 11;
	
	for(int i = 0; i < 10 ; i++) {
		if(cmd[1] == level[i]) {
			selectLevel = i;
		}
	}
	switch (selectLevel) {
        case 0:
            ModeIp(*channel);
			break;
        case 1:
            ModeIm(*channel);
            break;
        case 2:
            if(cmd.size() >= 4 && (!cmd[3].empty()))
                ModeKp(*channel, cmd[3]);
			break;
        case 3:
                ModeKm(*channel);
            break;
        case 4:
           if(cmd.size() >= 4 && (!cmd[3].empty()))
                ModeLp(*channel, cmd[3]);
			break;
        case 5:
            ModeLm(*channel);
            break;
        case 6:
            if(cmd.size() >= 4 && (!cmd[3].empty()))
                ModeOp(*channel, cmd[3]);
            break;
        case 7:
            ModeOm(*channel, cmd[3]);
            break;
        case 8:
            ModeTp(*channel);
            break;
        case 9:
            ModeTm(*channel);
            break;
        default:
			std::cout << "Please select a correct level !" << std::endl;
	}
	(void) password;
}
