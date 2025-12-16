#include "CmdBlackJack.hpp"
#include "Exception.hpp"
#include "Utils.hpp"

static std::vector<Channel*>::iterator	findChannel(std::vector<Channel*>& allChannels, std::string channel) {

    std::vector<Channel*>::iterator it = allChannels.begin();
    for (; it != allChannels.end(); it++) {
        if((*it)->getName() == channel)
	break ;
	}
	return it;
}

std::string intToString(int value) {
    std::stringstream ss;
    ss << value; 
    return ss.str();
}

CmdBlackJack::CmdBlackJack() {}


void setup(Channel& channel) {
	if(channel.getBJMode() == true)
		return;
	channel.setBJ(true);
	std::vector<User*> users = channel.getOperators();
	std::vector<User*>::iterator it = users.begin();
	for (; it != users.end(); it++)
		channel.demoteUser(*(*it));
	channel.setTopic("CHANNEL DU BLACKJACK");
	channel.setIsInviteOnly(false);
	channel.setIsKeyProtected(false);
	channel.setIsTopicProtected(true);
	channel.setIsLimit(true);
	channel.setUserLimit(100);
}

void handleCmd(Channel *channel, User *user, std::string cmd){
	if(channel->getBJMode() == false)
		return; //MESSAGE
    if (cmd == "START")
    {
        if(channel->getIsBJRun() == true){
			noticeReply(user, "Blackjack already running.");
			return;
		}
		channel->startBJ();
		noticeReply(channel, "Blackjack game is starting.");
		return;
    }
    else if (cmd == "HIT")
	{
		if (!channel->getIsBJRun())
		{
			noticeReply(user, "No blackjack game running.");
			return;
		}
		std::vector<Channel::BJPlayer>::iterator it = channel->getBJPlayers().begin();
		Channel::BJPlayer* p = NULL;
		for (; it != channel->getBJPlayers().end(); ++it)
		{
			if (it->user == user)
			{
				p = &(*it);
				break;
			}
		}
		if (!p)
		{
			noticeReply(user, "You are not in the game.");
			return;
		}
		if (p->stayed)
		{
			noticeReply(user, "You already stayed.");
			return;
		}
		int card = std::rand() % 13 + 1;
		p->hand.push_back(card);
		std::string message = "You drew: " + intToString(card);
		noticeReply(user, message);
	}
	else if (cmd == "STAY")
	{
		if (!channel->getIsBJRun())
		{
			noticeReply(user, "No blackjack game running.");
			return;
		}
		std::vector<Channel::BJPlayer>::iterator it = channel->getBJPlayers().begin();
		Channel::BJPlayer* p = NULL;
		for (; it != channel->getBJPlayers().end(); ++it)
		{
			if (it->user == user)
			{
				p = &(*it);
				break;
			}
		}
		if (!p)
		{
			noticeReply(user, "You are not in the game.");
			return;
		}
		p->stayed = true;
		noticeReply(user, "You chose to STAY.");
	}
	else if (cmd == "DOUBLE")
	{
		if (!channel->getIsBJRun())
		{
			noticeReply(user, "No blackjack game running.");
			return;
		}
		std::vector<Channel::BJPlayer>::iterator it = channel->getBJPlayers().begin();
		Channel::BJPlayer* p = NULL;
		for (; it != channel->getBJPlayers().end(); ++it)
		{
			if (it->user == user)
			{
				p = &(*it);
				break;
			}
		}
		if (!p)
		{
			noticeReply(user, "You are not in the game.");
			return;
		}
		if (p->hand.size() != 2)
		{
			noticeReply(user, "You can only DOUBLE on your first 2 cards.");
			return;
		}
		//p->bet *= 2; // gestion des mises
		int card = rand() % 13 + 1;
		p->hand.push_back(card);
		p->stayed = true;
		std::string message = "You doubled and drew: " + intToString(card);
		noticeReply(user, message);
	}
	else if (cmd == "IN")
	{
		if (!channel->getIsBJRun())
    	{
			noticeReply(user, "No blackjack game running.");
        	return;
    	}
    	if (channel->isBJPlayer(user))
    	{
			noticeReply(user, "You are already in the game.");
        	return;
		}
		noticeReply(channel, user->getNickname() + " joined the blackjack game.");
		channel->addBJPlayer(user);
		Channel::BJPlayer& p = channel->getBJPlayers().back();
		std::string message  = "Your cards : " + intToString(p.hand[0]) + " " + intToString(p.hand[1]);
		noticeReply(user, message);
	}
	else if (cmd == "OUT")
    {
      if (!channel->getIsBJRun())
        {
			noticeReply(user, "No blackjack game running.");
            return;
        }
		std::vector<Channel::BJPlayer>::iterator it = channel->getBJPlayers().begin();
        bool found = false;
        for (; it != channel->getBJPlayers().end(); ++it)
        {
            if (it->user == user)
            {
                channel->getBJPlayers().erase(it);
                found = true;
                break;
            }
        }
        if (!found)
        {
			noticeReply(user, "You are not in the game.");
            return;
        }
		noticeReply(channel, user->getNickname() + " has left the blackjack game.");
        if (channel->getBJPlayers().empty())
        {
            channel->stopBJ();
			noticeReply(channel, " :The blackjack game has ended (no more players).");
        }
        return;
	}
	else
    {
        throw ExceptionCode(ERR_UNKNOWNCOMMAND);
    }
}

void CmdBlackJack::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& password,
    std::vector<Channel*>& allChannels,
    std::map<int, User*>& fdToUser)
{
	std::cout << "GFUIDHGJD" << cmd[0] << std::endl;
    User *user = fdToUser[fd_origin];
    if (!isUserValidAuth(*user, 1, 1, 1))
        throw ExceptionCode(ERR_NOTREGISTERED);
    if (cmd.size() < 1)
        throw ExceptionCode(ERR_NEEDMOREPARAMS);
    std::vector<Channel*>::iterator it = findChannel(allChannels, cmd[0]);
    if (it == allChannels.end())
        throw ExceptionCode(ERR_NOSUCHCHANNEL);
    Channel *channel = *it;
    if (cmd.size() == 1)
    {
        setup(*channel);
        return;
    }
    handleCmd(channel, user, cmd[2]);
	(void) password;
}
