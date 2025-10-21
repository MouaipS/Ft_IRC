#include "KICK.hpp"
#include "algorithm"

/**
 * @brief Browse the channel's list of users and check if the given name exists.
 * @returns [1] Not found: nothing happens. [2] Found: the user is removed from the list.
 */
void Kick::execCmd(User &user, Channel &channel) {
	//parcourir std::vector<std::string> _usersname;	
	std::string name = user.getUsername();
	std::vector<std::string>::iterator it = std::find(channel.getUsersname().begin(), channel.getUsersname().end(), name);
	if(it == channel.getUsersname().end())
		std::cout << "NO USER WITH THIS NAME CAN BE KICK, JACKASS" << std::endl;
	else {
		channel.getUsersname().erase(it);
		std::cout << "USER KICK, JACKASS" << std::endl; 
	}
}



