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
 * @return size_t 
 * - L'indice (position) de l'utilisateur dans le vecteur `_users` s'il est trouvé.
 * - Retourne `-1` si l'utilisateur n'est pas présent dans le canal.
 */
size_t Channel::findUser(User& user)
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
 * @return size_t 
 * - L'indice (position) de l'utilisateur dans le vecteur `_operators` s'il est trouvé.
 * - Retourne `-1` si l'utilisateur n'est pas opérateur du canal.
 */
size_t Channel::findOperator(User& user)
{
	for (size_t i = 0; i < _operators.size(); ++i)
	{
		if (&user == _operators[i])
			return (i);
	}
	return (-1);
}

/**
 * @brief Ajoute un utilisateur au canal si possible.
 * 
 * @param user Référence vers l'utilisateur à ajouter.
 * @return bool 
 * - `true` si l'utilisateur a été ajouté avec succès.
 * - `false` si l'utilisateur est déjà présent ou si la limite d'utilisateurs est atteinte.
 */
bool Channel::addUserToChannel(User& user)
{
	if (_isUserLimit && _users.size() >= _userLimit)
		return (false);

	if (findUser(user) >= 0)
		return (false);

	_users.push_back(&user);
	return (true);
}

/**
 * @brief Retire un utilisateur du canal (et de la liste des opérateurs s'il en fait partie).
 * 
 * @param user Référence vers l'utilisateur à retirer.
 * @return bool 
 * - `true` si l'utilisateur a été retiré avec succès.
 * - `false` si l'utilisateur n'était pas présent dans le canal.
 */
bool Channel::removeUserFromChannel(User& user)
{
	size_t user_index;
	size_t operator_index;

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

/**
 * @brief Promeut un utilisateur au rôle d'opérateur du canal.
 * 
 * @param user Référence vers l'utilisateur à promouvoir.
 * @return bool 
 * - `true` si l'utilisateur a été promu avec succès.
 * - `false` si l'utilisateur n'existe pas dans le canal ou est déjà opérateur.
 */
bool Channel::promoteUser(User &user)
{
	if (findUser(user) < 0)
		return (false);

	if (findOperator(user) >= 0)
		return (false);

	_operators.push_back(&user);
	return (true);
}

/**
 * @brief Rétrograde un opérateur en simple utilisateur.
 * 
 * @param user Référence vers l'utilisateur à rétrograder.
 * @return bool 
 * - `true` si l'utilisateur a été retiré de la liste des opérateurs.
 * - `false` si l'utilisateur n'existe pas ou n'était pas opérateur.
 */
bool Channel::demoteUser(User &user)
{
	size_t operator_index;

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
