#include "CmdMode.hpp"
#include "Exception.hpp"
#include "Utils.hpp"

CmdMode::CmdMode() : ICommand::ICommand() {};

static std::vector<std::string> parseFlags(const std::string& flags)
{
    std::vector<std::string> result;
    char currentSign = '+';

    for (size_t i = 0; i < flags.size(); ++i)
    {
        char c = flags[i];
        if (c == '+' || c == '-') {
            currentSign = c;
            continue;
        }
        if (std::isalpha(c)) {
            std::string token;
            token += currentSign;
            token += c;
            result.push_back(token);
        }
    }
    return result;
}
void CmdMode::sendUsers(std::vector<User *> users, Channel *channel, std::string mode) {
	std::vector<User *>::iterator it = users.begin();
	for(; it != users.end(); it++) {
		serverReply((*it)->getFd(),  "MODE "+ channel->getName() + mode, 0);
	}
}

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
    if (cmd.size() < 3 || cmd[1].empty() || cmd[2].empty())
		throw ExceptionCode(ERR_NEEDMOREPARAMS);
    channel = findChannel(cmd[1], allChannels);
	if (!channel)
		throw ExceptionCode(ERR_NOSUCHCHANNEL);
    if (channel->findUser(*user) == -1) {
		throw ExceptionCode(ERR_NOTONCHANNEL);
    } else if (channel->findOperator(*user) == -1) {
		throw ExceptionCode(ERR_CHANOPRIVSNEEDED);
    }
    std::vector<std::string> flags = parseFlags(cmd[2]);
    std::vector<std::string>::iterator it = flags.begin();
    const std::string level[10] = {"+i","-i","+k","-k","+l","-l","+o","-o","+t","-t"};
	for(; it != flags.end();it++){
        int selectLevel = 11;
        for(int i = 0; i < 10 ; i++) {
	    	if(*it == level[i]) {
	    		selectLevel = i;
	    	}
	    }
	    switch (selectLevel) {
            case 0:
                ModeIp(*channel);
                sendUsers(channel->getUsers(), channel, " +i");
	    		break;
            case 1:
                ModeIm(*channel);
                sendUsers(channel->getUsers(), channel, " -i");
                break;
            case 2:
                if(cmd.size() >= 4 && (!cmd[3].empty())){
                    ModeKp(*channel, cmd[3]);
                    sendUsers(channel->getUsers(), channel, " +k " + cmd[3]);
                }
                else
                    throw ExceptionCode(ERR_NEEDMOREPARAMS);
	    		break;
            case 3:
                ModeKm(*channel);
                sendUsers(channel->getUsers(), channel, " -k");
                break;
            case 4:
               if(cmd.size() >= 4){
                    ModeLp(*channel, cmd[3]);
                    sendUsers(channel->getUsers(), channel, " +l " + cmd[3]);
                }
                else
                    throw ExceptionCode(ERR_NEEDMOREPARAMS);
	    		break;
            case 5:
                ModeLm(*channel);
                sendUsers(channel->getUsers(), channel, " -l");
                break;
            case 6:
                if(cmd.size() >= 4 && (!cmd[3].empty()))
                    ModeOp(*channel, cmd[3]);
                else
                    throw ExceptionCode(ERR_NEEDMOREPARAMS);
                break;
            case 7:
                if(cmd.size() >= 4 && (!cmd[3].empty()))
                    ModeOm(*channel, cmd[3]);
                else
                    throw ExceptionCode(ERR_NEEDMOREPARAMS);
                break;
            case 8:
                ModeTp(*channel);
                sendUsers(channel->getUsers(), channel, " +t ");
                break;
            case 9:
                ModeTm(*channel);
                sendUsers(channel->getUsers(), channel, " -t ");
                break;
            default:
	    	    throw ExceptionCode(ERR_UNKNOWNMODE, *it, channel->getName());
	    }
    }
	(void) password;
}
