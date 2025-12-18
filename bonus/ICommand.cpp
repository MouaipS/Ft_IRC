#include "ICommand.hpp"
#include <stdio.h>
#include <unistd.h>

ICommand::ICommand() {}

ICommand::~ICommand() {}

const char* ICommand::UserNotFoundException::what() const throw()
{
	return ("User not found in server list.");
}

const char* ICommand::ChannelNotFoundException::what() const throw()
{
	return ("Channel not found in server list.");
}
