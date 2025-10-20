#pragma once

#include <list>
#include "Channel.hpp"
#include "User.hpp"
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <bits/stdc++.h>
#include <fcntl.h>


#define CHANNEL_LIMIT	1000

class Server
{
	public:
		Server();
		Server(std::string name, std::string password);
		Server(const Server &obj);
		~Server();

		// OPERATORS
		// Server &operator=(const Server &obj);

		// FUNCTIONS
		void	userJoinServer(std::string userName);
		void	userJoinChannel(User &user, std::string chName);
		void	initServer(std::string portNumber);

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
		std::vector<int>		_client_fd;

		addrinfo			hints;
		addrinfo			*res;
		//sockaddr_storage	their_addr;
		int					sockfd;
		//int					acceptfd;

};
