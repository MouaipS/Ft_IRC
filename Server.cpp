#include "Server.hpp"

Server::Server(): _name("SilkRoad"), _password("motdepasse") {

	Channel	Home("home", "", "default channel");
	_allChannels.push_back(Home);

	User	Admin("Admin", "Admin", _allChannels.back());
	_allUsers.push_back(Admin);
}

Server::Server(std::string name, std::string password): _name(name), _password(password) {

	_allChannels.push_back(Channel("home", 0, "default channel"));
	_allUsers.push_back(User("Admin", "God", _allChannels.back()));
}

Server::Server(const Server &obj) {

	(void)obj;
}

Server::~Server() {}



// O P E R A T O R S

// Server &Server::operator=(const Server &obj) {

	// if (this != &obj) {
		// _name = obj._name;
		// _password = obj._password;
		// _allChannels = obj._allChannels;
		// _allUsers = obj._allUsers;
	// }
	// return (*this);
// }



// F U N C T I O N S

void	Server::userJoinServer(std::string userName) {

	int	size = _allUsers.size();

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

void	Server::initServer(std::string portNumber) {

	memset(&hints, 0, sizeof(hints));

	res = NULL;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, portNumber.c_str(), &hints, &res)) {
		std::cout << "Error: while getaddrinfo" << std::endl;
		return ;
	}

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
	if (sockfd == -1) {
		freeaddrinfo(res);
		std::cout << "Error: socket() failed" << std::endl;
		return ;
	}
	
	if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
		std::cout << "Error: bind() failed" << std::endl;
		return ;
	}

	// if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
		// std::cout << "Error: connect() failed" << std::endl;
		// freeaddrinfo(res);
		// return ;
	// }

	if (listen(sockfd, 10) == -1) {
		std::cout << "Error: listen() failed" << std::endl;
		freeaddrinfo(res);
		return ;
	}

	socklen_t	addr_size = sizeof(their_addr);
	acceptfd = accept(sockfd, (sockaddr *)&their_addr, &addr_size);
	if (acceptfd == -1) {
		std::cout << "Error: accept() failed" << std::endl;
		freeaddrinfo(res);
		return ;
	}
}



// E X C E P T I O N S

const char*	Server::ServerLimitUser::what() const throw() {
	return ("The server is full"); }

const char*	Server::UserNameAlreadyUsed::what() const throw() {
	return ("User name already used"); }

const char* Server::ServerLimitChannel::what() const throw() {
	return ("Can't create more channels"); }
