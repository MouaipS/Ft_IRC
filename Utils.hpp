#pragma once

#include <iostream>
#include "Server.hpp"

extern int sigStatus;

void	serverReply(int fd, std::string message, int flag);
void	clientReply(int, std::string, std::string, std::string, std::string, std::string, int);
bool	isUserValidAuth(User& user, bool pass, bool nick, bool username);
std::string	displayTimestamp(void);
std::string	getServerName();
