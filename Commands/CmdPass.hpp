
#include "ICommand.hpp"

class CmdPass : public ICommand
{
	public:
		std::map<std::vector<User>, std::string> execCmd (
					std::vector<std::string>&	cmd,
					const std::string&			name,
					const std::string&			password,
					std::vector<Channel>&		allChannels,
					std::map<User, int>&		fdToUser
		);

};