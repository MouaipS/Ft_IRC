#include "ICommand.hpp"
#include "CmdPart.hpp"
#include "Exception.hpp"
#include "Utils.hpp"

CmdPart::CmdPart() : ICommand::ICommand() {};

static Channel *findChannel(std::string channel, std::vector<Channel*>& allChannels) {

    std::vector<Channel*>::iterator it = allChannels.begin();
    for(; it != allChannels.end(); it++){
        if((*it)->getName() == channel)
            return((*it));
    }
	return 0;
}

static std::vector<std::string> parseChannel(std::string	chanToPart)
{
	std::vector<std::string>					tmp;
	std::stringstream							ss(chanToPart);
	std::string									buffer;

	while (getline(ss, buffer, ','))
		tmp.push_back(buffer);
	return (tmp);
}

void sendUsers(std::vector<User *> users, Channel *channel, User *user, std::string reason) {

	std::vector<User *>::iterator it = users.begin();
	for(; it != users.end(); it++) {
		clientReply((*it)->getFd(), user->getNickname(), user->getUsername(), "PART", channel->getName(), reason, 0);
	}
}

std::string	reasonPart(std::vector<std::string> &cmd) {

	std::string	reason;

	cmd[2].erase(cmd[2].begin());
	for (size_t i = 2; i < cmd.size(); i++)
	{
		reason += cmd[i];
		if (i == cmd.size() - 1)
			break ;
		reason += ' ';
	}

	return (reason);
}

void CmdPart::execCmd(
    int fd_origin, std::vector<std::string>& cmd, const std::string& password,
    std::vector<Channel*>& allChannels,std::map<int, User*>& fdToUser)
{
	(void)password;
	User *user = fdToUser[fd_origin];
	if (!isUserValidAuth(*user, 1,1,1))
		throw ExceptionCode(ERR_NOTREGISTERED);
	if (cmd.size() <= 1)
		throw ExceptionCode(ERR_NEEDMOREPARAMS);
	std::string reason = user->getNickname();
	if(cmd.size() > 2 )
	{
		if (cmd[2][0] != ':')
			throw ExceptionCode(ERR_INPUTTOOLONG);
		reason = reasonPart(cmd);
	}
	std::cout << "reason: " << reason << std::endl;
	
	std::vector<std::string> channels = parseChannel(cmd[1]);
	std::vector<std::string>::iterator it = channels.begin();

	for(; it != channels.end();it++)
	{
		Channel *channel;
		channel = findChannel((*it), allChannels);
		if (!channel)
		{
			throw ExceptionCode(ERR_NOSUCHCHANNEL);
		}
		if (channel->findUser(*user) == -1) {
			throw ExceptionCode(ERR_NOTONCHANNEL);
		}
		channel->removeUserFromChannel(*user);
		sendUsers(channel->getUsers(), channel, user, reason);
	}
}
