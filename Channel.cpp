#include "Channel.hpp"
#include "User.hpp"

Channel::Channel(std::string name) : _name(name) {}

std::string Channel::getName() const {
	return(_name);
}

void Channel::changeLimit(int limit) {
	if(_isLimit == true) {
		if(limit > 0 && limit < 100) {
			_limit = limit;
		}
		else 
			std::cout << "A Channel can't accept more than hundred user." << std::endl;
	}
	else
		std::cout << "You can't change the limit. Please enable it before making changes." << std::endl;
}

void Channel::setLimit() {
	_isLimit = !_isLimit;
	if(_isLimit == true) 
		std::cout << "Limit changed : enabled." << std::endl;
	else 
		std::cout << "Limit change : disabled." << std::endl;

}

void	Channel::addUser(User &user) {

	if (_usersname.size() == LIMIT_USER)
		throw UserLimitChannel();
	_usersname.push_back(user.getUsername());
}

const char*	Channel::UserLimitChannel::what() const throw() {
	return ("The channel is full");
}


std::vector<std::string> &Channel::getUsersname() {
	return(_usersname);
}

std::vector<std::string> &Channel::getOperator() {
	return(_operator);
}

bool Channel::changeInvite() {
	_invitMode = !_invitMode;
	return(_invitMode);
}

bool Channel::changeKey() {
	_KeyMode = !_KeyMode;
	return(_KeyMode);
}
