#include "CmdKick.hpp"

CmdKick::CmdKick(std::string serverName) : ICommand::ICommand(serverName) {};


bool checkTarget(std::string target, std::string channel, std::vector<Channel*>& allChannels, User *userTarget) {
    std::vector<Channel*>::iterator it = allChannels.begin();
    for(; it != allChannels.end(); it++){
        if((*it)->getName() == channel)
        {
            userTarget = (*it)->findUser(target);
            if(userTarget == NULL)
                return(false);
            return(true);
        }
    }
    return(false);
}

void CmdKick::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& password,
    std::vector<Channel*>& allChannels,
    std::map<int, User*>& fdToUser
) { // 451✅ 461✅ 403 441 481/482 353
    std::string reason;
    User *user = fdToUser[fd_origin];
    User *target;
    if(!isUserValidAuth(*user, 1,1,1)){
        sendToUser(fd_origin, "451 " + user->getNickname() + ":You have not registered", 0);
        return;
    }
    if(cmd.size() == 4) {
        reason = cmd[3];
    } else if(cmd.size() == 3) {
        reason = user->getNickname() + " kicked ";
        return;
    } else {
        sendToUser(fd_origin, "461 " + user->getUsername() + " KICK :Not enough parameters", 0);
        return;
    }

    if(checkTarget(cmd[1], cmd[2], allChannels, target) == false){
        sendToUser(fd_origin, "403 " + user.)
    }




    //Parsing des arguments (KICK userKick channel "mesage/ rien")


    //Check si user authentifier ✅
    //Check si il est dans un channel
    //Check si le gars a kick est dans le meme channel
    // Si tout est ok, kick le gars
    //Lister tout les messages possibles
    // TODO: Implémenter la commande Kick

    ////Erreur: "You are not channel operator"
    //::server 481 <nick> :You are not channel operator
    //
    ////Erreur: "No such nick/channel"
    //::server 403 <nick> <channel> :No such nick/channel
    //
    ////Erreur: "They aren't on that channel"
    //::server 441 <nick> <user> :They aren't on that channel
    //
    ////Erreur: "You're not channel operator"
    //::server 482 <nick> <channel> :You're not channel operator
    //
    ////Succès: Expulsion avec message
    //::server KICK <channel> <user> :<message>
    //
    ////Expulsion et application d'un ban
    //::server MODE <channel> +b <user!host>
    //
    ////Erreur: "User not in channel"
    //::server 441 <nick> <user> :They aren't on that channel
    //
    ////Erreur: "Channel is empty"
    //::server 353 <nick> <channel> :Channel is empty


	(void) fd_origin;
	(void) cmd;
	(void) password;
	(void) allChannels;
	(void) fdToUser;	
}
