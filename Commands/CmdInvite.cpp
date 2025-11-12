#include "CmdInvite.hpp"

CmdInvite::CmdInvite(): ICommand() {}

void CmdInvite::execCmd (	
			int							fd_origin,
			std::vector<std::string>&	cmd,
			const std::string&			password,
			std::vector<Channel*>&		allChannels,
			std::map<int, User*>&		fdToUser
			) {
    // TODO: Implémenter la commande Invite

    //Check si user auth, si dans le channel, si la target existe et n'est pas dans le channel, 
    //si invite-only, check si le user est modo
    //si pas invite only, juste add la target dans la channel list


	(void) fd_origin;
	(void) cmd;
	(void) password;
	(void) allChannels;
	(void) fdToUser;
}
