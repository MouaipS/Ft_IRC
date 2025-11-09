#include "CmdMode.hpp"


CmdMode::CmdMode(std::string serverName) : ICommand::ICommand(serverName) {};

Channel *findChannel(std::string channel, std::vector<Channel*>& allChannels){
    std::vector<Channel*>::iterator it = allChannels.begin();
    for(; it != allChannels.end(); it++){
        if((*it)->getName() == channel)
            return((*it));
    }
    throw ICommand::ChannelNotFoundException();
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
    if(!isUserValidAuth(*user, 1,1,1)){
        sendToUser(fd_origin, "451 " + user->getUsername() + " :You have not registered", 0);
        return;
    }
    if(cmd.size() < 3 || cmd[2].empty()){
        sendToUser(fd_origin, "461 " + user->getUsername() + " MODE :Not enough parameters", 0);
        return;
    }
    try{
        channel = findChannel(cmd[2], allChannels);
    }catch(std::exception& e){ sendToUser(fd_origin, "403 " + user->getUsername() + " :No such channel", 0); return;}
    if(channel->findUser(*user) == -1){
        sendToUser(fd_origin, "442 " + user->getUsername() + " :You are not on that channel", 0);
        return;
    } else if(channel->findOperator(*user) == -1){
        sendToUser(fd_origin, "482 " + user->getUsername() + " :You are not channel operator", 0);
        return;
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
            if(!cmd[3].empty())
                ModeKp(*channel, cmd[3]);
			break;
        case 3:
                ModeKm(*channel);
            break;
        case 4:
           if(!cmd[3].empty())
                ModeLp(*channel, cmd[3]);
			break;
        case 5:
            ModeLm(*channel);
            break;
        case 6:
            if(!cmd[3].empty())
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
