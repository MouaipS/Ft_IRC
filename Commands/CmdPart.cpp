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

static std::vector<std::string> parseChannel(const std::vector<std::string>& cmd)
{
    std::vector<std::string> result;
	std::vector<std::string>::const_iterator it = cmd.begin();
	it++;
    for (; it != cmd.end(); ++it)
    {
        std::string tmp;
        for (size_t i = 0; i < it->size(); ++i)
        {
            char c = (*it)[i];
            if (c == ' ' || c == ',')
            {
                if (!tmp.empty())
                {
                    result.push_back(tmp);
                    tmp.clear();
                }
            }
            else
                tmp += c;
        }
        if (!tmp.empty())
            result.push_back(tmp);
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
	
	std::vector<std::string> channels = parseChannel(cmd);
	std::vector<std::string>::iterator it = channels.begin();
	int i = -1;
	for(; it != channels.end();it++)
	{
		Channel *channel;
		std::cout <<"i = "<< i << " Nom du channel :" << (*it) <<std::endl;
		channel = findChannel((*it), allChannels);
		if (!channel)
			throw ExceptionCode(ERR_NOSUCHCHANNEL);
		i++;
		if (channel->findUser(*user) == -1) {
			throw ExceptionCode(ERR_NOTONCHANNEL);
		}
		channel->removeUserFromChannel(*user);
	}


}