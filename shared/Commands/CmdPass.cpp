#include "CmdPass.hpp"
#include "Utils.hpp"
#include "Exception.hpp"

CmdPass::CmdPass() : ICommand::ICommand() {};

static bool	verifyPassword(const std::string& value, const std::string& ref)
{
	return (value == ref);
}

void CmdPass::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& password,
    std::vector<Channel*>& allChannels,
    std::map<int, User*>& fdToUser)
{
	(void)allChannels;
	User* user = fdToUser[fd_origin];
	
	if (cmd.size() < 2)
		throw ExceptionCode(ERR_NEEDMOREPARAMS);

	if (user->getIsAuthed())
		throw ExceptionCode(ERR_ALREADYREGISTRED);

	if (!verifyPassword(cmd[1], password))
		throw ExceptionCode(ERR_PASSWDMISMATCH);
	
	user->setIsAuthed(true);
	(void) password;
}
