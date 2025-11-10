#include "CmdInvite.hpp"

CmdInvite::CmdInvite(std::string serverName) : ICommand::ICommand(serverName) {};

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
        if((*it).second->getUsername() == target)
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
    //INVITE TARGET CHANNEL
    // TODO: Implémenter la commande Invite

    //Check si user auth, si dans le channel, si la target existe et n'est pas dans le channel, 
    //si invite-only, check si le user est modo
    //si pas invite only, juste add la target dans la channel list
    Channel *channel;
    User *user = fdToUser[fd_origin];
    User *target = NULL;
    if(!isUserValidAuth(*user, 1,1,1)){
        sendToUser(fd_origin, "451 " + user->getUsername() + " :You have not registered", 0);
        return;
    }
    if(cmd.size() != 3 || cmd[1].empty() || cmd[2].empty()){
        sendToUser(fd_origin, "461 " + user->getUsername() + " MODE :Not enough parameters", 0);
        return;
    }
    try{
        channel = findChannel(cmd[2], allChannels);
    }catch(std::exception& e){ sendToUser(fd_origin, "403 " + user->getUsername() + " :No such channel", 0); return;}
    try{
        target = findTarget(cmd[1], fdToUser);
    }catch(std::exception& e){std::cout<< "TARGET EXISTE PAS" << std::endl; return;}
    if(channel->findUser(*target) != -1){
        std::cout << "TARGET DEJA PRESENT DANS LE CHANNEL" <<std::endl;
        sendToUser(1, "TARGET DEJA PRESENT DANS LE CHANNEL", 1);
        return;
    }
    if(channel->getIsInviteOnly() ==  true){
        if(channel->findOperator(*user) != -1){
            channel->addGuest(*target);
            std::cout << "INVITE POUR LA TARGET (MODE ACTIV)" <<std::endl;
            sendToUser(1, "INVITE POUR LA TARGET", 0);
        } else{
            std::cout << "PAS POSSIBLE CAR USER PAS OP" <<std::endl;
            sendToUser(1, "PAS POSSIBLE CAR USER PAS OP", 0);
        }
        return;
    }
    channel->addGuest(*target);
    std::cout << "INVITE POUR LA TARGET" <<std::endl;
    sendToUser(1, "INVITE POUR LA TARGET", 0);
	(void) password;
}
