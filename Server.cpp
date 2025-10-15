#include "Server.hpp"

Server::Server(): _name("SilkRoad"), _password("motdepasse") {

	_allChannels.push_back(Channel("home", 0, "default channel"));
	_allUsers.push_back(User("Admin", "God", _allChannels.back()));
}

Server::Server(std::string name, std::string password): _name(name), _password(password) {

	_allChannels.push_back(Channel("home", 0, "default channel"));
	_allUsers.push_back(User("Admin", "God", _allChannels.back()));
}

Server::Server(const Server &obj) {}

Server::~Server() {}



// -- O P E R A T O R S -- \\

Server &Server::operator=(const Server &obj) {

	if (this != &obj) {
		_name = obj._name;
		_password = obj._password;
		_allChannels = obj._allChannels;
		_allUsers = obj._allUsers;
	}
	return (*this);
}



// -- F U N C T I O N S -- \\

void	Server::userJoinServer(std::string userName) {

	int	size = _allUsers.size();

	if (size == USER_LIMIT)
		throw	ServerLimitUser();

	for (int i = 0; i < size; i++) {

		if (userName == _allUsers[i].getUsername())
			throw UserNameAlreadyUsed();
	}
	_allUsers.push_back(User(userName, userName, _allChannels.back()));
}

void	Server::userJoinChannel(User &user, std::string chName) {

	int	size = _allChannels.size();

	for (int i = 0; i < size; i++) {

		if (chName == _allChannels[i].getName()) {
			
			_allChannels[i].addUser(user);
			return ;
		}
	}
	if (_allChannels.size() == CHANNEL_LIMIT)
		throw ServerLimitChannel();
	_allChannels.push_back(Channel(chName, 0, 0));
}



// -- E X C E P T I O N S -- \\

const char*	Server::ServerLimitUser::what() const throw() {
	return ("The server is full"); }

const char*	Server::UserNameAlreadyUsed::what() const throw() {
	return ("User name already used"); }

const char* Server::ServerLimitChannel::what() const throw() {
	return ("Can't create more channels"); }
