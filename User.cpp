#include "User.hpp"

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
