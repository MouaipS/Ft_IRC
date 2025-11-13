#pragma once

#include <iostream>
#include "Server.hpp"

extern int sigStatus;

void	serverReply(int fd, std::string message, int flag);
void	clientReply(int, std::string, std::string, std::string, std::string, std::string, int);
void	sendToUser2(int fd, std::string message, std::string message2, int flag);
bool	isUserValidAuth(User& user, bool pass, bool nick, bool username);
std::string	getServerName();
