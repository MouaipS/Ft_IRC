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
    // TODO: Implémenter la commande Mode
    User *user = fdToUser[fd_origin];
    if(!isUserValidAuth(*user, 1,1,1)){
        sendToUser(fd_origin, "451 " + user->getUsername() + ":You have not registered", 0);
        return;
    }
    try{
        channel = findChannel(cmd[2], allChannels);
    }catch(std::exception& e){ sendToUser(fd_origin, "403 " + user->getUsername() + " :No such channel", 0);}
    if(channel->findUser(*user) == -1){
        sendToUser(fd_origin, "442 " + user->getUsername() + ":You are not on that channel", 0);
    } else if(channel->findOperator(*user) == -1){
        sendToUser(fd_origin, "482 " + user->getUsername() + ":You are not channel operator", 0);
    }

    const std::string level[8] = {"-i", "+i", "t", "+k", "-k", "o", "+l", "+l"};
	int selectLevel = 0;
	
	for(int i = 0; i < 8 ; i++) {
		if(cmd[1] == level[i]) {
			selectLevel = i;
		}
	}
	switch (selectLevel) {
        case 0:
            ModeIm(*channel);
			break;
        case 1:
            ModeIp(*channel);
            break;
        case 2:
            if(!cmd[3].empty())
                ModeKp(*channel, cmd[3]);
			break;
        case 3:
            if(!cmd[3].empty())
                ModeKm(*channel, cmd[3]);
            break;
        case 4:
                ModeLm(*channel);
			break;
        case 5:
            if(!cmd[3].empty())
            ModeLp(*channel, cmd[3]);
            break;
        case 6:
            ModeO();
            break;
        case 7:
            ModeT();
            break;
        default:
			std::cout << "Please select a correct level !" << std::endl;
	}

    //Check si channel existe, si user existe et OP, check le mode demande(et parsing des arguements)

    | Mode  | Effet                                         | Paramètre requis ?          |
| ----- | --------------------------------------------- | ------------------------        |
| **i** | Canal sur invitation seulement                |                           ❌    |
| **t** | Seuls les opérateurs peuvent changer le topic |                           ❌    |
| **k** | Définit un mot de passe                       |              ✅ (mot de passe)  |
| **o** | Donne ou retire le statut opérateur           |                      ✅ (nick)  |
| **l** | Définit une limite d’utilisateurs             |     ✅ pour `+l`, ❌ pour `-l`  |


	(void) fd_origin;
	(void) cmd;
	(void) password;
	(void) allChannels;
	(void) fdToUser;	
}
