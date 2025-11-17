#include "CmdJoin.hpp"
#include "Exception.hpp"
#include "Utils.hpp"

CmdJoin::CmdJoin() : ICommand::ICommand() {};

static bool	alreadyOnChannel(User* user, std::vector<User*> allUsers) {

	for (size_t i = 0; i < allUsers.size(); i++) {

		if (user->getNickname() == allUsers[i]->getNickname())
			return true;
	}
	return false;
}

static Channel *findChannel(std::vector<Channel*>& allChannels, std::string name) {

	std::vector<Channel*>::iterator it = allChannels.begin();
	for(;it != allChannels.end(); it++) {

		 if((*it)->getName() == name)
			break;
	}
	return(*it);
}

static void	leaveAllChannels(std::vector<Channel*>& allChannels, User&	user) {

	for (size_t i = 0; i < allChannels.size(); i++) {

		std::vector<User*>	allUsers = allChannels[i]->getUsers();
		for (size_t j = 0; j < allUsers.size(); j++) {

			if (allUsers[j]->getNickname() == user.getNickname()) {

				allChannels[i]->removeUserFromChannel(user);
				break ;
			}
		}
	}
}

static std::vector<std::string>	splitArgs(std::string chanToJoin) {

	std::vector<std::string>					cmd;
	std::stringstream							ss(chanToJoin);
	std::string									buffer;

	while (getline(ss, buffer, ','))
		cmd.push_back(buffer);
	return (cmd);
}


static bool findGuest(User *user, Channel* channel){
	const std::vector<User *>& guestlist = channel->getGuestList();
	std::vector<User *>::const_iterator it = guestlist.begin();
	for(; it != guestlist.end(); it++) {
		if(user->getUsername() == (*it)->getUsername())
			return true;
	}
	return false;
}

static void sendUsers(std::vector<User *> users, Channel *channel, std::string name) {
	std::vector<User *>::iterator it = users.begin();
	for(; it != users.end(); it++) {
		serverReply((*it)->getFd(), "353 = " + channel->getName() + " :" + name, 0);
	}
}

static void sendJoinReply(std::vector<User *> users, Channel *channel) {
	std::vector<User *>::iterator it = users.begin();
	for(; it != users.end(); it++) {
		serverReply((*it)->getFd(), "JOIN " + channel->getName(), 0);
	}
}

void CmdJoin::execCmd(
		int fd_origin,
		std::vector<std::string>& cmd,
		const std::string& password,
		std::vector<Channel*>& allChannels, 
		std::map<int, User*>& fdToUser )
{
	(void)	password;
	User*	user = fdToUser[fd_origin];

	if (!isUserValidAuth(*user, 1, 1, 1))
		throw ExceptionCode(ERR_PASSWDMISMATCH);
	if (cmd.size() == 1)
		throw ExceptionCode(ERR_NEEDMOREPARAMS);
	if (cmd[1].size() == 1 && cmd[1][0] == '0') {

		leaveAllChannels(allChannels, (*user));
		return ;
	}

	std::vector<std::string>	chanToJoin = splitArgs(cmd[1]);
	for (size_t i = 0; i < chanToJoin.size(); i++) {

		if (chanToJoin[i][0] != '#' && chanToJoin[i][0] != '&')
			throw ExceptionCode(ERR_NOSUCHCHANNEL);
	}

	std::vector<std::string>	passwords;
	if (cmd.size() == 3)
		passwords = splitArgs(cmd[2]);

	for (size_t j = 0; j < chanToJoin.size(); j++)
	{
		std::vector<Channel*>::iterator	it = allChannels.begin();
		for (; it != allChannels.end(); it++) {

			if ((*it)->getName() == chanToJoin[j])
			{
				if (alreadyOnChannel(user, (*it)->getUsers()))
						throw ExceptionCode(ERR_USERONCHANNEL);
				else
				{
					if ((*it)->getIsLimit()){
						if((*it)->getUsers().size() >= (*it)->getUserLimit())
							throw ExceptionCode(ERR_CHANNELISFULL, "", (*it)->getName());
					}
					if ((*it)->getIsKeyProtected())
					{
						if (passwords[j] != (*it)->getKey())
							throw ExceptionCode(ERR_PASSWDMISMATCH);
					}
					if((*it)->getIsInviteOnly()){
						if(!findGuest(user, (*it)))
							throw ExceptionCode(ERR_INVITEONLYCHAN, "", (*it)->getName());
						std::vector<User*>& guests = (*it)->getGuestList();
						guests.erase(std::remove(guests.begin(), guests.end(), user),guests.end());
					} else if(findGuest(user, (*it))) {
					std::vector<User*>& guests = (*it)->getGuestList();
					guests.erase(std::remove(guests.begin(), guests.end(), user),guests.end());
					}
					serverReply(fd_origin, "JOIN " + chanToJoin[j], 0);
					Channel *channel = findChannel(allChannels, chanToJoin[j]);
					channel->setNewUser(user);
					serverReply(fd_origin, "332 = " + chanToJoin[j] + " :" + channel->getTopic(), 0);
					
					std::string	tmp;
					for (size_t i = 0; i < channel->getUsers().size(); i++) {
						
						tmp += channel->getUsers()[i]->getNickname();
						if (i == channel->getUsers().size() - 1)
						break;
						tmp += ' ';
					}
					
					sendUsers(channel->getUsers(), channel, tmp);
					return ;
				}
			}
		}
		allChannels.push_back(new Channel(chanToJoin[j]));
		Channel *channel = findChannel(allChannels, chanToJoin[j]);
		channel->setNewUser(user);
		channel->promoteUser(*user);
		sendJoinReply(channel->getUsers(), channel);
		sendUsers(channel->getUsers(), channel, user->getNickname());
	}
}
