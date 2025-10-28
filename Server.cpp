#include "Server.hpp"
#include "ICommand.hpp"

Server::Server(std::string name, std::string password): _name(name), _password(password) {}

Server::~Server() {}

// F U N C T I O N S

void	Server::NewClient(int fd_actif, epoll_event dataEpoll, int epoll_fd) {
	int client_fd = accept(fd_actif, NULL, NULL);
	if(client_fd == -1){
		std::cout << "Error: accept() failed" << std::endl;
	} else {
		int flags = fcntl(client_fd, F_GETFL, 0);
		fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
		dataEpoll.data.fd = client_fd;
		epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &dataEpoll);
		//instancier nouveau user(par def) pour l'ajouter a la map avec son bon fd
		_client_fd.push_back(client_fd);
	}
}

void	Server::initCommands() {

	_commands.insert({"KICK", new CmdKick()});
	_commands.insert({"INVITE", new CmdInvite()});
	_commands.insert({"TOPIC", new CmdTopic()});
	_commands.insert({"MODE", new CmdMode()});
	_commands.insert({"JOIN", new CmdJoin()});
	_commands.insert({"NICK", new CmdNick()});
	_commands.insert({"PASS", new CmdPass()});
	_commands.insert({"PRIVMSG", new CmdPrivmsg()});
	_commands.insert({"USER", new CmdUser()});
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

	if (listen(sockfd, 10) == -1) {
		std::cout << "Error: listen() failed" << std::endl;
		freeaddrinfo(res);
		return ;
	}
	
	initCommands();
	std::vector<std::string>	args;
	std::map<int, std::string>	fdAndMessage;
	std::string					str;
	int epoll_fd = epoll_create(1);
	epoll_event dataEpoll, events[180];
	dataEpoll.events = EPOLLIN;
	dataEpoll.data.fd = sockfd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &dataEpoll);

	while(1) {
		int nb_event = epoll_wait(epoll_fd, events, 180, 10);
		if(nb_event != -1){
			for(int i = 0; i < nb_event; i++) {
				int fd_actif = events[i].data.fd;

				if(fd_actif == sockfd) {
					NewClient(fd_actif, dataEpoll, epoll_fd);
				} else {
					str = getBuffer();//GESTION MESSAGE + IDCHECK/PSS
					args = splitBuffer(str); //split les arguments sans les checker
					fdAndMessage = sendToCommand(args);
					sendToUsers(fdAndMessage);
				}
			}
		}
	}
}

std::vector<std::string> Server::splitBuffer(std::string str) {

	std::vector<std::string>					cmd;
	std::stringstream							ss(str);
	std::string									buffer;

	while (getline(ss, buffer, ' '))
		cmd.push_back(buffer);

	return (cmd);
}

std::map<int, std::string>	Server::sendToCommand(std::vector<std::string> cmd) {

	std::map<std::string, ICommand*>::iterator	it;

	it = _commands.find(cmd[0]);
	if (it == _commands.end())
		throw InvalidCommand();

	it->second->execCmd(cmd, _name, _password, _allChannels, _fdToUser);
}

void	Server::sendToUsers(std::map<int, std::string>) {

}

// E X C E P T I O N S

const char*	Server::ServerLimitUser::what() const throw() {
	return ("The server is full"); }

const char*	Server::UserNameAlreadyUsed::what() const throw() {
	return ("User name already used"); }

const char* Server::ServerLimitChannel::what() const throw() {
	return ("Can't create more channels"); }
