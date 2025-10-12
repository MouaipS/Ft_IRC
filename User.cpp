#include "User.hpp"

User::User(std::string username, std::string nickname, Channel home) : _username(username), _nickname(nickname) {
	_MyChannels.push_back(home);
}

User::~User() {}

/**
 * @return _username of the user
 */
std::string User::getUsername() const {
	return(_username);
}

/**
 * @return _nickname of the user
 */
std::string User::getNickname() const {
	return(_nickname);
}

/**
 * @warning No protection if user have no Channel cause all users should be in the default channel
 * @return std::vector : list of string "name of channel"
 */
std::vector<std::string> User::getChannel() const {
	std::vector<std::string> names;
	for (std::list<Channel>::const_iterator it = _MyChannels.begin(); it != _MyChannels.end(); ++it) {
		names.push_back(it->getName());
    }
	return(names);
}

void User::ChangeNickname(std::string nickname){
	_nickname = nickname;
}


std::ostream	&operator<<(std::ostream &fd, User *User) {
	fd << "User Info" << std::endl;
	fd << "Username : " << User->getUsername() << std::endl;
	fd << "Nickname : " << User->getNickname() << std::endl;
	fd << "Channel(s) : " << std::endl;
	
	std::vector<std::string> names = User->getChannel();
	for(std::vector<std::string>::const_iterator it = names.begin(); it != names.end(); ++it) {
		fd << "		□ " << names.data() << std::endl;
	}
	return (fd);
}