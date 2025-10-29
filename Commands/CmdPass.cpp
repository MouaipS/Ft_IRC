#include "CmdPass.hpp"

static bool	verifyPassword(const std::string& value, const std::string& ref)
{
	return (value == ref);
}

void CmdPass::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& name,
    const std::string& password,
    std::vector<Channel>& allChannels,
    std::map<int, User*>& fdToUser)
{
	User* user = fdToUser[fd_origin];

	if (cmd.size() < 2)
	{
		sendToUser(fd_origin, user->getNickname() + " PASS:Not enough parameters", 0);
		return ;
	}

	if (user->getIsAuthed())
	{
		sendToUser(fd_origin, user->getNickname() + " PASS:You may not reregister", 0);
		return ;
	}

	if (!verifyPassword(cmd[1], password))
	{
		sendToUser(fd_origin, user->getNickname() + " PASS:Password incorrect", 0);
		return ;
	}
	
	user->setIsAuthed(true);

    // TODO: Implémenter la commande Pass
	(void) fd_origin;
	(void) cmd;
	(void) name;
	(void) password;
	(void) allChannels;
	(void) fdToUser;	
}
