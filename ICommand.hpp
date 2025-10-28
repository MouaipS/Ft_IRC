#pragma once
#include "Channel.hpp"
#include "User.hpp"
#include <map>

class ICommand {

	public:
		virtual ~ICommand();
		virtual void execCmd (	
					int							fd,
					std::vector<std::string>&	cmd,
					const std::string&			name,
					const std::string&			password,
					std::vector<Channel>&		allChannels,
					std::map<int, User*>&		fdToUser
		) = 0;

		class UserNotFoundException: public std::exception {
			public: const char* what() const throw();
		};

	private:
		bool	isUserValidAuth(User& user, bool pass, bool nick, bool username);

};
