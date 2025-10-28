#include "CmdPass.hpp"

std::map<std::vector<User>, std::string> execCmd (
			int							fd,
			std::vector<std::string>&	cmd,
			const std::string&			name,
			const std::string&			password,
			std::vector<Channel>&		allChannels,
			std::map<int, User>&		fdToUser)
{
	std::map<int, User>::iterator it;

	it = fdToUser.find(fd);
	if (it == fdToUser.end())
		throw (ICommand::UserNotFoundException());
	
	User& user = it->second;
	if (!user.getIsAuthed())
		return ();
		
	if (cmd.size() != 2)
		return (make_usage_message(fd, fdToUser));
}