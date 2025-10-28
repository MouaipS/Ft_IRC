#pragma once
#include "Channel.hpp"
#include "User.hpp"
#include <map>

class ICommand {

	public:
		virtual std::map<std::vector<int>, std::string> execCmd (	
					int							fd,
					std::vector<std::string>&	cmd,
					const std::string&			name,
					const std::string&			password,
					std::vector<Channel>&		allChannels,
					std::map<User, int>&		fdToUser
		) = 0;

		class UserNotFoundException: public std::exception {
			public: const char* what() const throw();
		};

};
