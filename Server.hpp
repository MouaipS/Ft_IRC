#pragma once

#include <list>
#include "Channel.hpp"
#include "User.hpp"
#include <stdexcept>

#define CHANNEL_LIMIT	1000

class Server
{
	public:
		Server();
		Server(std::string name, std::string password);
		Server(const Server &obj);
		~Server();

		// OPERATORS
		Server &operator=(const Server &obj);

		// FUNCTIONS
		void	userJoinServer(std::string userName);
		void	userJoinChannel(User &user, std::string chName);

		// EXCEPTIONS
		class ServerLimitUser: public std::exception {
			public: const char* what() const throw(); };
		class UserNameAlreadyUsed: public std::exception {
			public: const char* what() const throw(); };
		class ServerLimitChannel: public std::exception {
			public: const char* what() const throw(); };

	private:
		std::string				_name;
		std::string				_password;
		std::vector<Channel>	_allChannels;
		std::vector<User>		_allUsers;
};
