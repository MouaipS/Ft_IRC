#include "ICommand.hpp"
#include "CmdPart.hpp"
#include "Exception.hpp"
#include "Utils.hpp"

static Channel *findChannel(std::string channel, std::vector<Channel*>& allChannels) {

    std::vector<Channel*>::iterator it = allChannels.begin();
    for(; it != allChannels.end(); it++){
        if((*it)->getName() == channel)
            return((*it));
    }
	return 0;
}

static std::vector<std::string> parsechannel(const std::vector<std::string>& cmd)
{
    std::vector<std::string> result;

    std::vector<std::string>::const_iterator it;
    for (it = cmd.begin(); it != cmd.end(); ++it)
    {
        std::istringstream iss(*it);
        std::string token;

        while (iss >> token)
            result.push_back(token);
    }
    return result;
}


void CmdPart::execCmd(
    int fd_origin, std::vector<std::string>& cmd, const std::string& password,
    std::vector<Channel*>& allChannels,std::map<int, User*>& fdToUser)
{
	(void)password;


	 ///PART channel , channel, channel, 'Part message'


	 User *user = fdToUser[fd_origin];
	 if (!isUserValidAuth(*user, 1,1,1))
	 throw ExceptionCode(ERR_NOTREGISTERED);
	 if (cmd.size() <= 1)
	 throw ExceptionCode(ERR_NEEDMOREPARAMS);
	
	std::vector<std::string> channels = parsechannel(cmd);
	std::vector<std::string>::iterator it = channels.begin();
	for(; it != channels.end();it++)
	{
		Channel *channel;
		channel = findChannel(cmd[1], allChannels);
		if (!channel)
			throw ExceptionCode(ERR_NOSUCHCHANNEL);
		if (channel->findUser(*user) == -1) {
			throw ExceptionCode(ERR_NOTONCHANNEL);
		}
		channel->removeUserFromChannel(*user);
	}


}