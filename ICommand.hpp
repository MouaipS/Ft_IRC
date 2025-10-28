#pragma once
#include "Channel.hpp"
#include "User.hpp"
#include <map>

class ICommand {

	public:
		virtual ~ICommand();
		virtual std::map<std::vector<User>, std::string> execCmd (	
					std::vector<std::string>&	cmd,
					const std::string&			name,
					const std::string&			password,
					std::vector<Channel>&		allChannels,
					std::map<User, int>&		fdToUser
		) = 0;

	private :
		//recoit les arguments et doit les parser
};
