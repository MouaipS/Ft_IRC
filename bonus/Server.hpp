#pragma once

#include <list>
#include "Channel.hpp"
#include "ICommand.hpp"
#include "User.hpp"
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <bits/stdc++.h>
#include <fcntl.h>
#include "sys/epoll.h"
#include "Commands/CmdKick.hpp"
#include "Commands/CmdInvite.hpp"
#include "Commands/CmdTopic.hpp"
#include "Commands/CmdMode.hpp"
#include "Commands/CmdJoin.hpp"
#include "Commands/CmdNick.hpp"
#include "Commands/CmdPass.hpp"
#include "Commands/CmdPrivmsg.hpp"
#include "Commands/CmdUser.hpp"
#include "Commands/CmdPart.hpp"
#include "Commands/CmdBlackJack.hpp"

#define CHANNEL_LIMIT	1000
#define BUFFER_SIZE 512

enum command {

	KICK,
	INVITE,
	TOPIC,
	MODE,
	JOIN,
	NICK,
	PASS,
	PRIVMSG,
	USER
};

class Server
{
	public:
		Server(std::string port, std::string password);
		~Server();

		void	initServer(std::string portNumber);
		void	epollServer();
		void	initCommands();

		// EXCEPTIONS
		class ServerLimitUser: public std::exception {
			public: const char* what() const throw(); };
		class UserNameAlreadyUsed: public std::exception {
			public: const char* what() const throw(); };
		class ServerLimitChannel: public std::exception {
			public: const char* what() const throw(); };
		class NullCommand: public std::exception {
			public: const char* what() const throw(); };
		class GetAddrInfoFail: public std::exception {
			public: const char* what() const throw(); };
		class SocketFail: public std::exception {
			public: const char* what() const throw(); };
		class BindFail: public std::exception {
			public: const char* what() const throw(); };
		class ListenFail: public std::exception {
			public: const char* what() const throw(); };
		class PasswordRules: public std::exception {
			public: const char* what() const throw(); };
		class PortOutOfRange: public std::exception {
			public: const char* what() const throw(); };
		class InitCommandFail: public std::exception {
			public: const char* what() const throw(); };

	private:
		const std::string					_port;
		const std::string					_password;
		std::vector<Channel*>				_allChannels;
		std::map<int, User*>				_fdToUser;
		std::map<std::string, ICommand*>	_commands;
		bool								_isBJChannel;

		std::string			_timeOfDay;
		addrinfo			_hints;
		addrinfo			*_res;
		int					_sockfd;
		int					epoll_fd;
		epoll_event			_events[180];

		// FUNCTIONS
		void	sendToCommand(std::vector<std::string> cmd, int fd_origin);
		void	NewClient(int fd_actif, epoll_event dataEpoll, int epoll_fd);
		std::vector<std::string>	splitBuffer(User* user);
		void	updateUserBuffer(int fd_actif, User* user);
		void	handle_event(epoll_event event, epoll_event dataEpoll, int epoll_fd);
		User*	getUser(int fd);
		bool 	isBufferReady(std::string& buffer);
		void 	handleBufferTooLong(int fd, User *user, std::string& buffer);
		void 	cleanChannel();
		bool getBJ();
		void setBJ(bool status);

};
