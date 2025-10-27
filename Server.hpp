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
#include "sys/epoll.h"



#define CHANNEL_LIMIT	1000
#define SERVERNAME "irCnews"

class Server
{
	public:
		Server(std::string name, std::string password);
		~Server();

		// EXCEPTIONS
		class ServerLimitUser: public std::exception {
			public: const char* what() const throw(); };
		class UserNameAlreadyUsed: public std::exception {
			public: const char* what() const throw(); };
		class ServerLimitChannel: public std::exception {
			public: const char* what() const throw(); };

	private:
		const std::string				_name;
		const std::string				_password;  //fonction du H
		std::vector<Channel>			_allChannels;
		std::map<User, int>				_fdToUser;

		addrinfo			hints;
		addrinfo			*res;
		int					sockfd;

		// FUNCTIONS
		void	initServer(std::string portNumber);
		void	NewClient(int fd_actif, epoll_event dataEpoll, int epoll_fd);


};
