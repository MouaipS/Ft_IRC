#include "CmdSee.hpp"
#include <algorithm>

Cmdsee::Cmdsee(std::string serverName) : ICommand::ICommand(serverName) {};

#define RESET   "\033[0m"
#define CYAN    "\033[36m"
#define YELLOW  "\033[33m"
#define GREEN   "\033[32m"
#define MAGENTA "\033[35m"
#define BOLD    "\033[1m"

void printUsers(const std::vector<User*>& users, const std::string& title) {
    std::cout << BOLD << CYAN << "\n┌──────────────────── " << title << " ────────────────────┐" << RESET << std::endl;

    for (std::vector<User*>::const_iterator it = users.begin(); it != users.end(); ++it) {
        std::cout << GREEN << "│ Auth    : " << RESET << (*it)->getIsAuthed() << std::endl;
        std::cout << GREEN << "│ Username: " << RESET << (*it)->getUsername() << std::endl;
        std::cout << GREEN << "│ Nick    : " << RESET << (*it)->getNickname() << std::endl;
        std::cout << CYAN  << "├───────────────────────────────────────────────────────────" << RESET << std::endl;
    }

    std::cout << CYAN << "└───────────────────────────────────────────────────────────┘\n" << RESET << std::endl;
}


void Cmdsee::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& password,
    std::vector<Channel*>& allChannels,
    std::map<int, User*>& fdToUser
) {
	(void) fd_origin;
	(void) cmd;
	(void) password;
	(void) fdToUser;

	for (std::vector<Channel*>::iterator it = allChannels.begin(); it != allChannels.end(); ++it) {
		if (!*it) continue;

		std::cout << BOLD << MAGENTA 
        << "\n══════════════════════ CHANNEL INFO ═════════════════════" 
        << RESET << std::endl;

		std::cout << YELLOW << "Name            : " << RESET << (*it)->getName() << std::endl;
		std::cout << YELLOW << "Topic           : " << RESET << (*it)->getTopic() << std::endl;
		std::cout << YELLOW << "Key             : " << RESET << (*it)->getKey() << std::endl;
		std::cout << YELLOW << "Invite only     : " << RESET << (*it)->getIsInviteOnly() << std::endl;
		std::cout << YELLOW << "Topic protected : " << RESET << (*it)->getIsTopicProtected() << std::endl;
		std::cout << YELLOW << "Key protected   : " << RESET << (*it)->getIsKeyProtected() << std::endl;
		std::cout << YELLOW << "Limit enabled   : " << RESET << (*it)->getIsLimit() << std::endl;
		std::cout << YELLOW << "User limit      : " << RESET << (*it)->getUserLimit() << std::endl;

		printUsers((*it)->getGuestList(), "GUEST LIST");
		printUsers((*it)->getUsers(), "CONNECTED USERS");
		printUsers((*it)->getOperators(), "CHANNEL OPERATORS");
	}
}

