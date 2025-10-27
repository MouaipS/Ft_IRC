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
 * @brief Recherche un utilisateur dans la liste des utilisateurs du canal.
 * 
 * @param user Référence vers l'utilisateur à rechercher.
 * @return int
 * - L'indice (position) de l'utilisateur dans le vecteur `_users` s'il est trouvé.
 * - Retourne `-1` si l'utilisateur n'est pas présent dans le canal.
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
 * @brief Recherche un utilisateur dans la liste des opérateurs du canal.
 * 
 * @param user Référence vers l'utilisateur à rechercher.
 * @return int
 * - L'indice (position) de l'utilisateur dans le vecteur `_operators` s'il est trouvé.
 * - Retourne `-1` si l'utilisateur n'est pas opérateur du canal.
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
 * @brief Ajoute un utilisateur au canal.
 * 
 * @param user Référence vers l'utilisateur à ajouter.
 * @throw ChannelUserLimitExceededException si la limite d'utilisateurs est atteinte.
 * @throw UserAlreadyInChannelException si l'utilisateur est déjà présent dans le canal.
 * 
 * Ajoute l'utilisateur à la liste `_users` si le canal n'a pas atteint sa limite
 * et que l'utilisateur n'est pas déjà présent.
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
 * @brief Retire un utilisateur du canal (et de la liste des opérateurs s'il en fait partie).
 * 
 * @param user Référence vers l'utilisateur à retirer.
 * @throw UserNotFoundInChannelException si l'utilisateur n'est pas présent dans le canal.
 * 
 * Supprime l'utilisateur de la liste `_users`, et également de `_operators`
 * si celui-ci faisait partie des opérateurs.
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
 * @brief Promeut un utilisateur au rôle d'opérateur du canal.
 * 
 * @param user Référence vers l'utilisateur à promouvoir.
 * @throw UserNotFoundInChannelException si l'utilisateur n'est pas présent dans le canal.
 * @throw UserAlreadyOperatorException si l'utilisateur est déjà opérateur.
 * 
 * Ajoute l'utilisateur à la liste `_operators` s'il fait partie du canal
 * et qu'il n'est pas déjà opérateur.
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
 * @brief Rétrograde un opérateur en simple utilisateur.
 * 
 * @param user Référence vers l'utilisateur à rétrograder.
 * @throw UserNotFoundInChannelException si l'utilisateur n'est pas présent dans le canal.
 * @throw UserNotOperatorException si l'utilisateur n'est pas opérateur.
 * 
 * Supprime l'utilisateur de la liste `_operators` s'il est trouvé.
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
