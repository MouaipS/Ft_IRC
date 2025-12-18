#pragma once
#include "ICommand.hpp"

class CmdInvite : public ICommand {
public:
	CmdInvite();
	void execCmd (	
			int							fd,
			std::vector<std::string>&	cmd,
			const std::string&			password,
			std::vector<Channel*>&		allChannels,
			std::map<int, User*>&		fdToUser
			);
};
