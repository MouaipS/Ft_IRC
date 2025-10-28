#pragma once
#include "Channel.hpp"
#include "User.hpp"
#include <map>

class ICommand {

	public:
		virtual ~ICommand();
		virtual std::map<int, std::string> execCmd (	
					int							fd,
					std::vector<std::string>&	cmd,
					const std::string&			name,
					const std::string&			password,
					std::vector<Channel>&		allChannels,
					std::map<int, User>&		fdToUser
		) = 0;

		class UserNotFoundException: public std::exception {
			public: const char* what() const throw();
		};

};
