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

size_t	Channel::findUser(User& user)
{
	for (size_t i = 0; i < _users.size(); ++i)
	{
		if (&user == _users[i])
			return (i);
	}

	return (-1);
}

size_t	Channel::findOperator(User& user)
{
	for (size_t i = 0; i < _operators.size(); ++i)
	{
		if (&user == _operators[i])
			return (i);
	}

	return (-1);
}

bool Channel::addUserToChannel(User& user)
{
	if (_isUserLimit && _users.size() >= _userLimit)
		return (false);

	if (findUser(user) >= 0)
		return (false);

	_users.push_back(&user);

	return (true);
}

bool Channel::removeUserFromChannel(User& user)
{
	size_t	user_index;
	size_t	operator_index;

	user_index = findUser(user);
	operator_index = findOperator(user);

	if (operator_index >= 0)
		_operators.erase(_operators.begin() + operator_index);

	if (user_index >= 0)
	{
		_users.erase(_users.begin() + user_index);
		return (true);
	}

	return (false);
}

bool Channel::promoteUser(User &user)
{
	if (findUser(user) < 0)
		return (false);

	if (findOperator(user) >= 0)
		return (false);

	_operators.push_back(&user);

	return (true);
}

bool Channel::demoteUser(User &user)
{
	size_t	operator_index;

	if (findUser(user) < 0)
		return (false);

	operator_index = findOperator(user);
	if (operator_index >= 0)
	{
		_operators.erase(_operators.begin() + operator_index);
		return (true);
	}

	return (false);
}