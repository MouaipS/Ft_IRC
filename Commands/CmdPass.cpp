#include "CmdPass.hpp"

static std::string getFormatedMessage(std::string servName, std::string code, std::string nick, std::string mess)
{
	std::string message = ":";
	message += servName + " " + code + " " + nick + " PASS:" + mess + "\r\n";
	return (message);
}

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
		sendToUser(fd, ":" + name));
		return ;
	}

	if (user->getIsAuthed())
	{
		handle_already_registered(fd_origin, fdToUser[fd_origin], name);
		return ;
	}

	if (!verifyPassword(cmd[1], password))
	{
		handle_
	}
	

    // TODO: Implémenter la commande Pass
	(void) fd_origin;
	(void) cmd;
	(void) name;
	(void) password;
	(void) allChannels;
	(void) fdToUser;	
}
