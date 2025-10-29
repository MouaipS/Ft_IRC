#include "CmdPass.hpp"

void CmdPass::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& name,
    const std::string& password,
    std::vector<Channel>& allChannels,
    std::map<int, User*>& fdToUser)
{
	User* user = fdToUser[fd_origin];

	if (cmd.size() != 2)
		

	if (user->getIsAuthed())
	{

	}
	

    // TODO: Implémenter la commande Pass
	(void) fd_origin;
	(void) cmd;
	(void) name;
	(void) password;
	(void) allChannels;
	(void) fdToUser;	
}
