#include "Channel.hpp"
#include "User.hpp"

Channel::Channel(std::string name) : _name(name)
{
	_isInviteOnly = true;
	_isTopicProtected = true;
	_isKeyProtected = false;
	_isUserLimit = true;
	_userLimit = 100;
}

Channel::~Channel()
{
	std::cout << "Channel " << _name << "has been destroyed." << std::endl;
}

// ------------- GET  ------------- //
std::string Channel::getName() const
{
    return _name;
}

std::vector<User*> Channel::getUsers() const
{
    return _users;
}

std::vector<User*> Channel::getOperators() const
{
    return _operators;
}

std::string Channel::getTopic() const
{
    return _topic;
}

std::string Channel::getKey() const
{
    return _key;
}

bool Channel::getIsInviteOnly() const
{
    return _isInviteOnly;
}

bool Channel::getIsTopicProtected() const
{
    return _isTopicProtected;
}

bool Channel::getIsKeyProtected() const
{
    return _isKeyProtected;
}

bool Channel::getIsLimit() const
{
    return _isUserLimit;
}

size_t Channel::getUserLimit() const
{
    return _userLimit;
}


// ------------- SET  ------------- //

void Channel::setIsInviteOnly(bool value)
{
    _isInviteOnly = value;
}

void Channel::setTopic(const std::string& value)
{
    _topic = value;
}

void Channel::setIsTopicProtected(bool value)
{
    _isTopicProtected = value;
}

void Channel::setKey(const std::string& value)
{
    _key = value;
}

void Channel::setIsKeyProtected(bool value)
{
    _isKeyProtected = value;
}

void Channel::setUserLimit(size_t value)
{
    _userLimit = value;
}

void Channel::setIsLimit(bool value)
{
    _isUserLimit = value;
}


// ------------- UTILS  ------------- //

/**
 * @brief Searches for a user in the channel's user list.
 * 
 * @param user Reference to the user to search for.
 * @return int
 * - The index (position) of the user in the `_users` vector if found.
 * - Returns `-1` if the user is not present in the channel.
 */
int Channel::findUser(User& user)
{
	for (size_t i = 0; i < _users.size(); ++i)
	{
		if (&user == _users[i])
			return (i);
	}
	return (-1);
}

/**
 * @brief Searches for a user in the channel's operator list.
 * 
 * @param user Reference to the user to search for.
 * @return int
 * - The index (position) of the user in the `_operators` vector if found.
 * - Returns `-1` if the user is not an operator in the channel.
 */
int Channel::findOperator(User& user)
{
	for (size_t i = 0; i < _operators.size(); ++i)
	{
		if (&user == _operators[i])
			return (i);
	}
	return (-1);
}

/**
 * @brief Adds a user to the channel.
 * 
 * @param user Reference to the user to add.
 * @throw ChannelUserLimitExceededException if the user limit has been reached.
 * @throw UserAlreadyInChannelException if the user is already in the channel.
 * 
 * Adds the user to the `_users` list if the channel has not reached its limit
 * and if the user is not already present.
 */
void Channel::addUserToChannel(User& user)
{
	if (_isUserLimit && _users.size() >= _userLimit)
		throw (ChannelUserLimitExceededException());

	if (findUser(user) >= 0)
		throw (UserAlreadyInChannelException());

	_users.push_back(&user);
}

/**
 * @brief Removes a user from the channel (and from the operator list if applicable).
 * 
 * @param user Reference to the user to remove.
 * @throw UserNotFoundInChannelException if the user is not present in the channel.
 * 
 * Removes the user from the `_users` list, and also from `_operators`
 * if they were an operator.
 */
void Channel::removeUserFromChannel(User& user)
{
	size_t user_index;
	size_t operator_index;

	user_index = findUser(user);
	operator_index = findOperator(user);

	if (operator_index >= 0)
		_operators.erase(_operators.begin() + operator_index);

	if (user_index < 0)
		throw (UserNotFoundInChannelException());

	_users.erase(_users.begin() + user_index);
}

/**
 * @brief Promotes a user to channel operator.
 * 
 * @param user Reference to the user to promote.
 * @throw UserNotFoundInChannelException if the user is not present in the channel.
 * @throw UserAlreadyOperatorException if the user is already an operator.
 * 
 * Adds the user to the `_operators` list if they are part of the channel
 * and not already an operator.
 */
void Channel::promoteUser(User &user)
{
	if (findUser(user) < 0)
		throw (UserNotFoundInChannelException());

	if (findOperator(user) >= 0)
		throw (UserAlreadyOperatorException());

	_operators.push_back(&user);
}

/**
 * @brief Demotes an operator to a regular user.
 * 
 * @param user Reference to the user to demote.
 * @throw UserNotFoundInChannelException if the user is not present in the channel.
 * @throw UserNotOperatorException if the user is not an operator.
 * 
 * Removes the user from the `_operators` list if found.
 */
void Channel::demoteUser(User &user)
{
	size_t operator_index;

	if (findUser(user) < 0)
		throw (UserNotFoundInChannelException());

	operator_index = findOperator(user);
	if (operator_index < 0)
		throw (UserNotOperatorException());

	_operators.erase(_operators.begin() + operator_index);
}

// Exceptions

const char* Channel::ChannelUserLimitExceededException::what() const throw()
{
    return ("Channel user limit exceeded.");
}

const char* Channel::UserNotFoundInChannelException::what() const throw()
{
    return ("User not found in channel.");
}

const char* Channel::UserNotOperatorException::what() const throw()
{
    return ("User is not an operator.");
}

const char* Channel::UserAlreadyInChannelException::what() const throw()
{
    return ("User is already in the channel.");
}

const char* Channel::UserAlreadyOperatorException::what() const throw()
{
    return ("User is already an operator.");
}
