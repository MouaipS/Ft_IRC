#include "CmdInvite.hpp"
#include "Utils.hpp"
#include "Exception.hpp"

CmdInvite::CmdInvite(): ICommand() {}

static Channel *findChannel(std::string channel, std::vector<Channel*>& allChannels){
    std::vector<Channel*>::iterator it = allChannels.begin();
    for(; it != allChannels.end(); it++){
        if((*it)->getName() == channel)
            return((*it));
    }
    throw ICommand::ChannelNotFoundException();
}

static User *findTarget(std::string target, std::map<int, User*>& fdToUser){
    std::map<int, User*> ::iterator it = fdToUser.begin();
    for(;it != fdToUser.end(); it++){
        if((*it).second->getNickname() == target)
            return((*it).second);
    }
    throw ICommand::UserNotFoundException();
}

void CmdInvite::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& password,
    std::vector<Channel*>& allChannels,
    std::map<int, User*>& fdToUser
) {

    Channel *channel;
    User *user = fdToUser[fd_origin];
    User *target = NULL;
    if(!isUserValidAuth(*user, 1,1,1))
        throw ExceptionCode(ERR_NOTREGISTERED);
    if(cmd.size() != 3 || cmd[1].empty() || cmd[2].empty())
        throw ExceptionCode(ERR_NEEDMOREPARAMS);
    try{
        channel = findChannel(cmd[2], allChannels);
    }catch(std::exception& e){ throw ExceptionCode(ERR_NOSUCHCHANNEL);}
    if(channel->findUser(*user) == -1)
        throw ExceptionCode(ERR_NOTONCHANNEL);
    try{
        target = findTarget(cmd[1], fdToUser);
    }catch(std::exception& e){throw ExceptionCode(ERR_NOSUCHNICK);}
    if(channel->findUser(*target) != -1)
        throw ExceptionCode(ERR_USERONCHANNEL);
    if(channel->getIsInviteOnly() ==  true){
        if(channel->findOperator(*user) != -1){
            channel->addGuest(*target);
            clientReply(target->getFd(), user->getNickname(), user->getUsername(), "INVITE" , target->getNickname() + " " + channel->getName(), "", 0);
            serverReply(user->getFd(), channel->getName() + " " + target->getNickname(), 0);
        } else{
            throw ExceptionCode(ERR_CHANOPRIVSNEEDED, "", channel->getName());
        }
        return;
    }
    channel->addGuest(*target);
    clientReply(target->getFd(), user->getNickname(), user->getUsername(), "INVITE" , target->getNickname() + " " + channel->getName(), "", 0);
    serverReply(user->getFd(), channel->getName() + " " + target->getNickname(), 0);
	(void) password;
}
