#include "CmdBlackJack.hpp"
#include "Exception.hpp"
#include "Utils.hpp"

std::vector<Channel*>::iterator	findChannel(std::vector<Channel*>& allChannels, std::string channel) {

    std::vector<Channel*>::iterator it = allChannels.begin();

    for (; it != allChannels.end(); it++) {
        if((*it)->getName() == channel)
	break ;
	}
	return it;
}

CmdBlackJack::CmdBlackJack() {}


void setup(Channel *channel) {
	if(channel->getBJMode() == true)
		return;
	channel->setBJ(true);
	std::vector<User*>::iterator it = channel->getOperators();
	for (; it != channel->getOperators.end(); ++it)
		channel->demoteUser(it);
	channel->setTopic("CHANNEL DU BLACKJACK");
	channel->setIsInviteOnly(false);
	channel->setIsKeyProtected(false);
	channel->setIsTopicProtected(true);
	channel->setIsLimit(true);
	channel->setUserLimit(100);
}

void handleCmd(Channel *channel, User *user, std::string cmd){
	if(channel->getBJMode() == false)
		return; //MESSAGE
    if (cmd == "START")
    {
        if(channel->getIsBJRun() == true){

			NoticeClient(user->getFd(), ":server NOTICE " + channel->getName() + " :Blackjack already running.")
			return;
		}
		channel->startBJ();
		NoticeClient(TOUT LES FD, ":server NOTICE " + channel->getName() + " :Blackjack already running.")
		return;
    }
    else if (cmd == "HIT")
	{
		if (!channel->isBJRunning())
		{
			NoticeClient(user->getFd(), ":server NOTICE " + channel->getName() + " :No blackjack game running.");
			return;
		}
		std::vector<BJPlayer>& players = channel->getBJPlayers();
		BJPlayer* p = NULL;
		for (std::vector<BJPlayer>::iterator it = players.begin(); it != players.end(); ++it)
		{
			if (it->user == user)
			{
				p = &(*it);
				break;
			}
		}
		if (!p)
		{
			NoticeClient(user->getFd(), ":server NOTICE " + channel->getName() + " :You are not in the game.");
			return;
		}
		if (p->stayed)
		{
			NoticeClient(user->getFd(), ":server NOTICE " + channel->getName() + " :You already stayed.");
			return;
		}
		int card = std::rand() % 13 + 1;
		p->hand.push_back(card);
		NoticeClient(user->getFd(), "You drew: " + Utils::toString(card));
	}
	else if (cmd == "STAY")
	{
		if (!channel->isBJRunning())
		{
			NoticeClient(user->getFd(), ":server NOTICE " + channel->getName() + " :No blackjack game running.");
			return;
		}
		std::vector<BJPlayer>& players = channel->getBJPlayers();
		BJPlayer* p = NULL;

		for (std::vector<BJPlayer>::iterator it = players.begin(); it != players.end(); ++it)
		{
			if (it->user == user)
			{
				p = &(*it);
				break;
			}
		}
		if (!p)
		{
			NoticeClient(user->getFd(), ":server NOTICE " + channel->getName() + " :You are not in the game.");
			return;
		}
		p->stayed = true;
		NoticeClient(user->getFd(), ":server NOTICE " + channel->getName() + " :You chose to STAY.");
	}
	else if (cmd == "DOUBLE")
	{
		if (!channel->isBJRunning())
		{
			NoticeClient(user->getFd(), ":server NOTICE " + channel->getName() + " :No blackjack game running.");
			return;
		}
		std::vector<BJPlayer>& players = channel->getBJPlayers();
		BJPlayer* p = NULL;
		for (std::vector<BJPlayer>::iterator it = players.begin(); it != players.end(); ++it)
		{
			if (it->user == user)
			{
				p = &(*it);
				break;
			}
		}
		if (!p)
		{
			NoticeClient(user->getFd(), ":server NOTICE " + channel->getName() + " :You are not in the game.");
			return;
		}
		if (p->hand.size() != 2)
		{
			NoticeClient(user->getFd(), ":server NOTICE " + channel->getName() + " :You can only DOUBLE on your first 2 cards.");
			return;
		}
		p->bet *= 2; // si tu gères les mises
		int card = rand() % 13 + 1;
		p->hand.push_back(card);
		p->stayed = true;
		NoticeClient(user->getFd(), "You doubled and drew: " + Utils::toString(card));
	}
	else if (cmd == "IN")
	{
		if (!channel->isBJRunning())
    	{
			NoticeClient(user->getFd(), ":server NOTICE " + channel->getName() + "No blackjack game running.")
        	return;
    	}
    	if (channel->isBJPlayer(user))
    	{
			NoticeClient(user->getFd(), ":server NOTICE " + channel->getName() + "You are already in the game.")
        	return;
		}
		channel->addBJPlayer(user);
		NoticeClient(TOUT LES FD, ":server NOTICE " + channel->getName() + " :" + user->getNickname() + " joined the blackjack game.");
		BJPlayer& p = channel->getBJPlayers().back();
    	NoticeClient(user->getFd(),"Your cards: " +p.hand[0] + ", " +p.hand[1]);
	}
	else if (cmd == "END")
    {
      if (!channel->getIsBJRun())
        {
            NoticeClient(user->getFd(), ":server NOTICE " + channel->getName() + " :No blackjack game running.");
            return;
        }
        std::vector<BJPlayer>& players = channel->getBJPlayers();
        bool found = false;
        for (std::vector<BJPlayer>::iterator it = players.begin(); it != players.end(); ++it)
        {
            if (it->user == user)
            {
                players.erase(it);
                found = true;
                break;
            }
        }
        if (!found)
        {
            NoticeClient(user->getFd(), ":server NOTICE " + channel->getName() + " :You are not in the game.");
            return;
        }
        NoticeClient(TOUT_LES_FD, ":server NOTICE " + channel->getName() + " :" + user->getNickname() + " has left the blackjack game.");
        if (_bjPlayers.empty())
        {
            channel->stopBJ();
            NoticeClient(TOUT_LES_FD, ":server NOTICE " + channel->getName() + " :The blackjack game has ended (no more players).");
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
    User *user = fdToUser[fd_origin];
    if (!isUserValidAuth(*user, 1, 1, 1))
        throw ExceptionCode(ERR_NOTREGISTERED);
    if (cmd.size() < 2)
        throw ExceptionCode(ERR_NEEDMOREPARAMS);
    std::vector<Channel*>::iterator it = findChannel(allChannels, cmd[1]);
    if (it == allChannels.end())
        throw ExceptionCode(ERR_NOSUCHCHANNEL);
    Channel *channel = *it;
    if (cmd.size() == 2)
    {
        setup(channel);
        return;
    }
    handleCmd(channel, user, cmd[2]);
}
