#pragma once
#include "Channel.hpp"
#include "User.hpp"
#include <sys/socket.h>
#include <map>

class ICommand {

	public:
		virtual void execCmd (	
					int							fd,
					std::vector<std::string>&	cmd,
					const std::string&			password,
					std::vector<Channel>&		allChannels,
					std::map<int, User*>&		fdToUser
		) = 0;

		class UserNotFoundException: public std::exception {
			public: const char* what() const throw();
		};

		bool	isUserValidAuth(User& user, bool pass, bool nick, bool username);
		void	sendToUser(int fd, std::string message, int flag);
	
	protected:
		const std::string _serverName;
};
