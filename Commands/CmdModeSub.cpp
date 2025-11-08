#include "CmdMode.hpp"

void CmdMode::ModeIm(Channel &channel){
	channel.setIsInviteOnly(false);
}

void CmdMode::ModeIp(Channel &channel){
	channel.setIsInviteOnly(true);
}

void CmdMode::ModeKp(Channel &channel, std::string password){
	channel.setIsKeyProtected(true);
	channel.setKey(password);
}

void CmdMode::ModeKm(Channel &channel, std::string password){
	channel.setIsKeyProtected(false);
}

void CmdMode::ModeLm(Channel &channel){
	channel.setIsLimit(false);
}

void CmdMode::ModeLp(Channel &channel, std::string limit){
	channel.setIsLimit(true);
	int newLimit = std::atoi(limit.c_str());
	channel.setUserLimit(newLimit);
}

void CmdMode::ModeO(){
	
}




void CmdMode::ModeT(){}