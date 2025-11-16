#include "CmdMode.hpp"
#include "Utils.hpp"
#include "Exception.hpp"
#include <climits>

bool isValidLimit(const std::string &str, int &out)
{
    if (str.empty())
        return false;
    for (size_t i = 0; i < str.size(); ++i){
        if (!isdigit(str[i]))
            return false;
    }
    long long result = 0;
    for (size_t i = 0; i < str.size(); ++i) {
        int digit = str[i] - '0';
        result = result * 10 + digit;

        if (result > INT_MAX)
            return false;
    }

    if (result <= 1)
        return false;
    out = static_cast<int>(result);
    return true;
}


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

void CmdMode::ModeKm(Channel &channel){
	channel.setIsKeyProtected(false);
}

void CmdMode::ModeLm(Channel &channel){
	channel.setIsLimit(false);
}

void CmdMode::ModeLp(Channel &channel, std::string limit){
	int newLimit;
	if(isValidLimit(limit,newLimit) == false)
	{
		throw ExceptionCode(ERR_NEEDMOREPARAMS);
		return;
	}
	channel.setIsLimit(true);
	channel.setUserLimit(newLimit);
}

void CmdMode::ModeOp(Channel &channel, std::string target){
	User *userTarget;
	userTarget = channel.findUser(target);
	if(userTarget == NULL) {
		throw ExceptionCode(ERR_USERNOTINCHANNEL);
	}
	if(channel.findOperator(*userTarget) != -1){
		sendUsers(channel.getUsers(), &channel, " +o "+ userTarget->getUsername());
		return;
	}
	channel.promoteUser(*userTarget);
	sendUsers(channel.getUsers(), &channel, " +o "+ userTarget->getUsername());
}

void CmdMode::ModeOm(Channel &channel, std::string target){
	User *userTarget;
	userTarget = channel.findUser(target);
	if(userTarget == NULL) {
		throw ExceptionCode(ERR_USERNOTINCHANNEL);
	}
	if(channel.findOperator(*userTarget) == -1){
		sendUsers(channel.getUsers(), &channel, " -o "+ userTarget->getUsername());
		return;
	}
	channel.demoteUser(*userTarget);
	sendUsers(channel.getUsers(), &channel, " -o "+ userTarget->getUsername());
}

void CmdMode::ModeTp(Channel &channel){
	channel.setIsTopicProtected(true);
}

void CmdMode::ModeTm(Channel &channel){
	channel.setIsTopicProtected(false);
}
