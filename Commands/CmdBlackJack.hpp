#pragma once
#include "ICommand.hpp"

class CmdBlackJack : public ICommand {
public:
	CmdBlackJack();
	void execCmd (	
			int							fd,
			std::vector<std::string>&	cmd,
			const std::string&			password,
			std::vector<Channel*>&		allChannels,
			std::map<int, User*>&		fdToUser
			);
};

	//Si c'est pas la commande !BJ et que pas de channel -> rien ne se passe
	//Si c'est !BJ et le channel existe deja alors on join
	//Si le channel existe pas on le cree et on le JOIN

	//liste des commandes possibles :
		//!BJ WALLET
		//!BJ HIT
		//!BJ STAY
		//!BJ DOUBLE
		//!BJ IN
		//!BJ OUT


//////////////BULLSHIT/////////////		"
// int calculateScore(const std::vector<int>& hand)
// {
//     int score = 0;
//     int aceCount = 0;

//     for (std::vector<int>::const_iterator it = hand.begin(); it != hand.end(); ++it)
//     {
//         int card = *it;
//         if (card == 1) // As
//         {
//             score += 11;
//             aceCount++;
//         }
//         else if (card >= 10) // 10, J, Q, K
//             score += 10;
//         else
//             score += card;
//     }

//     while (score > 21 && aceCount > 0)
//     {
//         score -= 10;
//         aceCount--;
//     }

//     return score;
// }




//bool allStayed = true;
//std::vector<BJPlayer>& players = channel->getBJPlayers();
//for (std::vector<BJPlayer>::iterator it = players.begin(); it != players.end(); ++it)
//{
//    if (!it->stayed)
//    {
//        allStayed = false;
//        break;
//    }
//}
//if (allStayed)
//{
//    int bestScore = 0;
//    std::vector<BJPlayer*> winners;
//    for (std::vector<BJPlayer>::iterator it = players.begin(); it != players.end(); ++it)
//    {
//        int score = calculateScore(it->hand);
//        if (score > 21) continue;
//        if (score > bestScore)
//        {
//            bestScore = score;
//            winners.clear();
//            winners.push_back(&(*it));
//        }
//        else if (score == bestScore)
//        {
//            winners.push_back(&(*it));
//        }
//    }
//    if (winners.empty())
//    {
//        NoticeClient(TOUT_LES_FD, ":server NOTICE " + channel->getName() + " :All players busted! No winners.");
//    }
//    else
//    {
//        std::string msg = ":server NOTICE " + channel->getName() + " :Winner(s): ";
//        for (std::vector<BJPlayer*>::iterator it = winners.begin(); it != winners.end(); ++it)
//        {
//            msg += (*it)->user->getNickname() + " ";
//        }
//        NoticeClient(TOUT_LES_FD, msg);
//    }
//    channel->stopBJ();
//}

