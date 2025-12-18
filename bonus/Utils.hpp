#pragma once

#include <iostream>
#include "Server.hpp"

extern int sigStatus;

void			serverReply(int fd, std::string message, int flag);
void			clientReply(int, std::string, std::string, std::string, std::string, std::string, int);
bool			isUserValidAuth(User& user, bool pass, bool nick, bool username);
void			noticeReply(Channel *channel, std::string message);
void			noticeReply(User *user, std::string message);
std::string		displayTimestamp(void);
std::string		getServerName();
