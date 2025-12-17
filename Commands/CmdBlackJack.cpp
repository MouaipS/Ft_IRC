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

bool isNaturalBlackjack(const std::vector<int>& hand)
{
    if (hand.size() != 2)
        return false;
    int a1 = hand[0];
    int a2 = hand[1];
    bool hasAce = (a1 == 1 || a2 == 1);
    bool hasTen = (a1 >= 10 || a2 >= 10);
    return (hasAce && hasTen);
}


int getHandValue(const std::vector<int>& hand){
 	int sum = 0;
    int ace = 0;
    for (size_t i = 0; i < hand.size(); ++i)
    {
        int card = hand[i];
        if (card == 1)
        {
            sum += 11;
            ace++;
        }
        else if (card >= 10)
            sum += 10;
        else
            sum += card;
    }
    while (sum > 21 && ace > 0)
    {
        sum -= 10;
        ace--;
    }
    return sum;
}

bool allPlayersDone(Channel *channel)
{
    std::vector<Channel::BJPlayer>& players = channel->getBJPlayers();
    for (size_t i = 0; i < players.size(); i++)
    {
        if (!players[i].wait_end)
            return false;
    }
    return true;
}


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
	if(channel->getBJMode() == false){
		noticeReply(user, "Please setup the game with #BLACKJACK");
		return;
	}	
    if (cmd == "START")
    {
        if(channel->getIsBJRun() == true){
			noticeReply(user, "Blackjack already running.");
			return;
		}
		channel->startBJ();
		noticeReply(channel, "Blackjack game is starting.");
		channel->clearDealerHand();
    	int card1 = std::rand() % 13 + 1;
    	int card2 = std::rand() % 13 + 1;
    	channel->addDealerCard(card1);
   		channel->addDealerCard(card2);
    	noticeReply(channel, "Blackjack game is starting.");
    	noticeReply(channel, "Dealer shows: " + intToString(card1));
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
		if (p->wait_end)
		{
    		noticeReply(user, "You must wait for the end of the round.");
   			return;
		}
		if (p->stayed)
		{
			noticeReply(user, "You already stayed.");
			return;
		}
		if (p->blackjack)
		{
   			noticeReply(user, "You have a Blackjack. You cannot play.");
    		return;
		}
		int card = std::rand() % 13 + 1;
		p->hand.push_back(card);
		int res = getHandValue(p->hand);
		std::string message = "You drew: " + intToString(card);
		noticeReply(user, message);
		if (res > 21){
    		p->wait_end = true;
    		noticeReply(user, "Bust! Your total is over 21.");
		}
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
		if (p->wait_end)
		{
    		noticeReply(user, "You must wait for the end of the round.");
   			return;
		}
		if (p->blackjack)
		{
   			noticeReply(user, "You have a Blackjack. You cannot play.");
    		return;
		}
		p->stayed = true;
		p->wait_end = true;
		noticeReply(user, "You choose to STAY.");
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
		if (p->wait_end)
		{
    		noticeReply(user, "You must wait for the end of the round.");
   			return;
		}
		if (p->hand.size() != 2)
		{
			noticeReply(user, "You can only DOUBLE on your first 2 cards.");
			return;
		}
		if (p->blackjack)
		{
   			noticeReply(user, "You have a Blackjack. You cannot play.");
    		return;
		}
		    if (user->_wallet < p->bet)
   		{
        	noticeReply(user, "You don't have enough money to double.");
        	return;
    	}
    	user->_wallet -= p->bet;
    	p->bet *= 2;
		int card = rand() % 13 + 1;
		p->hand.push_back(card);
		p->stayed = true;
		p->wait_end = true;
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
		int bet = 10;
    	if (user->_wallet < bet)
    	{
       		noticeReply(user, "You don't have enough money to join the game.");
        	return;
    	}
    	user->_wallet -= bet;
		noticeReply(channel, user->getNickname() + " joined the blackjack game.");
		channel->addBJPlayer(user);
		Channel::BJPlayer& p = channel->getBJPlayers().back();
		p.stayed = false;
		p.wait_end = false;
		p.blackjack = false;
		p.bet = bet;
		std::string message  = "Your cards : " + intToString(p.hand[0]) + " " + intToString(p.hand[1]);
		if (isNaturalBlackjack(p.hand)){
			p.blackjack = true;
    		p.wait_end = true;
			noticeReply(user, message);
    		noticeReply(user, "BLACKJACK! You got a natural 21.");
		} else 
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
	else if(cmd == "CARDS"){
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
		std::string message  = "Your cards : ";
		for(size_t i = 0; i != p->hand.size(); i++){
			message += intToString(p->hand[i]) + " ";
		}
		noticeReply(user, message);
	}
	else
    {
        throw ExceptionCode(ERR_UNKNOWNCOMMAND);
    }
}

void autoEnd(Channel *channel)
{
    if (!channel->getIsBJRun() || !allPlayersDone(channel) || channel->getBJPlayers().size() == 0)
        return;

    std::vector<int>& dealer = channel->getDealerHand();

    std::string reveal = "Dealer hand: ";
    for (size_t i = 0; i < dealer.size(); i++)
        reveal += intToString(dealer[i]) + " ";
    noticeReply(channel, reveal);

    while (getHandValue(dealer) < 17)
    {
        int card = std::rand() % 13 + 1;
        dealer.push_back(card);
        noticeReply(channel, "Dealer draws: " + intToString(card));
    }

    int dealerScore = getHandValue(dealer);
    noticeReply(channel, "Dealer total: " + intToString(dealerScore));

    std::vector<Channel::BJPlayer>& players = channel->getBJPlayers();
    for (size_t i = 0; i < players.size(); i++)
    {
        Channel::BJPlayer& p = players[i];
        int playerScore = getHandValue(p.hand);

        std::string result;

		int payout = 0;
        if (playerScore > 21)
            result = "LOSE (bust)";
        else if (dealerScore > 21)
		{
            result = "WIN (dealer bust)";
        	payout = p.bet * 2;
		}
        else if (playerScore > dealerScore){
            result = "WIN";
        	payout = p.bet * 2;
		}
        else if (playerScore < dealerScore)
            result = "LOSE";
        else{
            result = "PUSH";
			payout = p.bet;
		}
		p.user->_wallet += payout;
        noticeReply(p.user, "Your total: " + intToString(playerScore) + " | Dealer: " + intToString(dealerScore) + " → " + result + " | Wallet: " + intToString(p.user->_wallet));
    }
    channel->stopBJ();
    noticeReply(channel, "Blackjack round ended.");
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
    handleCmd(channel, user, cmd[1]);
	autoEnd(channel);
	(void) password;
}
