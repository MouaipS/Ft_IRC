#include "Channel.hpp"
#include "User.hpp"

Channel::Channel(std::string name, std::string key, std::string topic) : _name(name), _key(key), _topic(topic), _isLimit(false), _isKey(true) {}

std::string Channel::getName() const {
	return(_name);
}

/**
 * @return Null if _isKey == false !!!
 */
std::string Channel::getKey() const {
	if(_isKey == true)
		return(_key);
	else
		return(NULL);
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

void Channel::setKey(std::string key) {
	if(key == "0") {
		_isKey = false;
		std::cout << "Key remove" << std::endl;
	}
	else {
		_isKey = true;
		_key = key;
		std::cout << "New key set." << std::endl;
	} 
}

void	Channel::addUser(User &user) {

	if (_usersname.size() == LIMIT_USER)
		throw UserLimitChannel();
	_usersname.push_back(user.getUsername());
}

const char*	Channel::UserLimitChannel::what() const throw() {
	return ("The channel is full");
}
