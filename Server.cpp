#include "Server.hpp"
#include "ICommand.hpp"

Server::Server(std::string port, std::string password): _port(port), _password(password) {
	// CHECK if password is valid (exampl, 12 charactere mini)
	// CHECK if port is in range, ex: 2000-65000
	// QUIT if not
	initServer(_port);
}

Server::~Server() {}

// F U N C T I O N S

void	Server::NewClient(int fd_actif, epoll_event dataEpoll, int epoll_fd) {
	int client_fd = accept(fd_actif, NULL, NULL);
	if(client_fd == -1)
	{
		std::cout << "Error: accept() failed" << std::endl;
		return ;
	}

	int flags = fcntl(client_fd, F_GETFL, 0);
	fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);
	dataEpoll.data.fd = client_fd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &dataEpoll);
	_fdToUser.insert(std::make_pair(client_fd, new User()));
}

void	Server::initCommands() {

	_commands.insert(std::make_pair("KICK", new CmdKick()));
	_commands.insert(std::make_pair("INVITE", new CmdInvite()));
	_commands.insert(std::make_pair("TOPIC", new CmdTopic()));
	_commands.insert(std::make_pair("MODE", new CmdMode()));
	_commands.insert(std::make_pair("JOIN", new CmdJoin()));
	_commands.insert(std::make_pair("NICK", new CmdNick()));
	_commands.insert(std::make_pair("PASS", new CmdPass()));
	_commands.insert(std::make_pair("PRIVMSG", new CmdPrivmsg()));
	_commands.insert(std::make_pair("USER", new CmdUser()));

	//TODO Il faut check si une des commandes est NULL et arreter le serveur si tel est le cas
}

void	Server::initServer(std::string port) {

	memset(&hints, 0, sizeof(hints));

	res = NULL;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, port.c_str(), &hints, &res)) {
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
					str = getBuffer(fd_actif);//GESTION MESSAGE + IDCHECK/PSS
					args = splitBuffer(str); //split les arguments sans les checker
					sendToCommand(args, fd_actif);
				}
			}
		}
	}
}

std::string	Server::getBuffer(int fd_actif) {

	int			rcvBytes;
	char		buffer[BUFFER_SIZE];
	std::string	buffString;

	// Alors, actually, sans vouloir t'enfoncer:
	/*
	When reading messages from a stream, read the incoming data into a buffer.
	Only parse and process a message once you encounter the \r\n at the end of it.
	If you encounter an empty message, silently ignore it.
	*/
	// Il faut un buffer pour chaque fd,e t traiter le message uniquement une fois \r\n
	rcvBytes = recv(fd_actif, buffer, sizeof(buffer) -2, 0);
	std::cout << rcvBytes << std::endl;
	buffer[rcvBytes - 1] = '\r'; //C'EST QUOI TA MERDE CA A PAS DE SENS : When reading messages from a stream, read the incoming data into a buffer. Only parse and process a message once you encounter the \r\n at the end of it. If you encounter an empty message, silently ignore it.
	buffer[rcvBytes] = '\n';

	buffString = buffer;
	return (buffString);
}

std::vector<std::string> Server::splitBuffer(std::string str) {

	std::vector<std::string>					cmd;
	std::stringstream							ss(str);
	std::string									buffer;

	while (getline(ss, buffer, ' '))
		cmd.push_back(buffer);

	return (cmd);
}

void	Server::sendToCommand(std::vector<std::string> cmd, int fd_origin)
{
	std::map<std::string, ICommand*>::iterator	it = _commands.find(cmd[0]);

	if (it == _commands.end())
		send(fd_origin, "Unknown command\r\n", 18, 0); // Format unknown command
	else
		it->second->execCmd(fd_origin, cmd, SERVERNAME, _password, _allChannels, _fdToUser);
}

// E X C E P T I O N S

const char*	Server::ServerLimitUser::what() const throw() {
	return ("The server is full"); }

const char*	Server::UserNameAlreadyUsed::what() const throw() {
	return ("User name already used"); }

const char* Server::ServerLimitChannel::what() const throw() {
	return ("Can't create more channels"); }
