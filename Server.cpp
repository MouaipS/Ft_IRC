#include "Server.hpp"
#include "ICommand.hpp"
#include <vector>

Server::Server(std::string port, std::string password): _port(port), _password(password) {}

Server::~Server() {
	/*
	std::map<std::string, ICommand*>::iterator it = _commands.begin();
	for (;it != _commands.end(); ++it)
		delete it->second;
	*/
}

// F U N C T I O N S

void	Server::NewClient(int fd_actif, epoll_event dataEpoll, int epoll_fd) {

	int client_fd = accept(fd_actif, NULL, NULL);

	if (client_fd == -1) {

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
}

void	Server::initServer(std::string port) {

	memset(&_hints, 0, sizeof(_hints));

	_res = NULL;
	_hints.ai_family = AF_UNSPEC;
	_hints.ai_socktype = SOCK_STREAM;
	_hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, port.c_str(), &_hints, &_res))
		throw GetAddrInfoFail();

	_sockfd = socket(_res->ai_family, _res->ai_socktype, _res->ai_protocol);
	if (_sockfd == -1) {
		freeaddrinfo(_res);
		throw SocketFail();
	}
	
	else if (bind(_sockfd, _res->ai_addr, _res->ai_addrlen) == -1) {
		freeaddrinfo(_res);
		throw BindFail();
	}

	else if (listen(_sockfd, 10) == -1) {
		freeaddrinfo(_res);
		throw ListenFail();
	}
}

User*	Server::getUser(int fd)
{
	std::map<int, User*>::iterator it;

	it = _fdToUser.find(fd);

	if (it == _fdToUser.end())
		return (NULL);

	return (it->second);
}

bool Server::isBufferReady(std::string& buffer)
{
	size_t	pos = buffer.find("\r\n");

	if (pos != std::string::npos && pos <= 510)
		return (true);

	return (false);
}

void Server::handleBufferTooLong(int fd, User *user, std::string& buffer)
{
	if (!user)
		return ;

	buffer.clear();
	std::string message = ":";
	message = message + SERVERNAME + " 417 " + user->getNickname() + " :Input line was too long\r\n";
	send(fd, message.c_str(), message.length(), 0);
}

void	Server::handle_event(epoll_event event, epoll_event dataEpoll, int epoll_fd)
{
	int fd_actif = event.data.fd;

	if (fd_actif == _sockfd)
	{
		NewClient(fd_actif, dataEpoll, epoll_fd);
		return ;
	}

	User	*user = getUser(fd_actif);
	if (user == NULL)
		return ;

	updateUserBuffer(fd_actif, user);
	user = getUser(fd_actif);
	if (user == NULL)
		return ;

	std::string& userBuffer = user->getBuffer();
	if (isBufferReady(userBuffer))
	{
		userBuffer.resize(userBuffer.size() - 2);
		std::vector<std::string> args = splitBuffer(user);
		for (std::vector<std::string>::iterator it = args.begin(); it != args.end(); ++it) {
        	std::cout << *it << std::endl;
    	}
		sendToCommand(args, fd_actif);
		userBuffer.clear();
	} 
	else if (userBuffer.size() > 510)
		handleBufferTooLong(fd_actif, user, userBuffer);
}

void	Server::epollServer()
{
	int	epoll_fd = epoll_create(1);

	epoll_event dataEpoll, events[180];
	dataEpoll.events = EPOLLIN;
	dataEpoll.data.fd = _sockfd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, _sockfd, &dataEpoll);

	while (1)
	{
		int nb_event = epoll_wait(epoll_fd, events, 180, 10);
		if (nb_event == -1)
			continue ;

		for (int i = 0; i < nb_event; i++)
			handle_event(events[i], dataEpoll, epoll_fd);
	}
}

void	Server::updateUserBuffer(int fd_actif, User* user) {

	int			rcvBytes;
	char		buffer[BUFFER_SIZE];
	std::string	buffString;

	if (!user)
		return ;
	std::memset(buffer, '\0', BUFFER_SIZE);
	rcvBytes = recv(fd_actif, buffer, sizeof(buffer), 0);
	if (rcvBytes == -1)
		return ;

	if (rcvBytes == 0)
	{
		delete user;
		_fdToUser.erase(fd_actif);
		return ;
	}

	std::string& userBuffer = user->getBuffer();
	userBuffer += buffer;
}

std::vector<std::string> Server::splitBuffer(User* user) {

	std::vector<std::string>					cmd;
	std::stringstream							ss(user->getBuffer());
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
		it->second->execCmd(fd_origin, cmd, _password, _allChannels, _fdToUser);
}

// E X C E P T I O N S

const char*	Server::ServerLimitUser::what() const throw() {
	return ("The server is full"); }

const char*	Server::UserNameAlreadyUsed::what() const throw() {
	return ("User name already used"); }

const char* Server::ServerLimitChannel::what() const throw() {
	return ("Can't create more channels"); }

const char* Server::NullCommand::what() const throw() {
	return ("Failed during commands initialisation"); }

const char* Server::GetAddrInfoFail::what() const throw() {
	return ("getaddrinfo() failed"); }

const char* Server::SocketFail::what() const throw() {
	return ("socket() failed"); }

const char* Server::BindFail::what() const throw() {
	return ("bind() failed"); }

const char* Server::ListenFail::what() const throw() {
	return ("listen() failed"); }

const char* Server::PasswordRules::what() const throw() {
	return ("Password must be at least 12 characters long"); }

const char* Server::PortOutOfRange::what() const throw() {
	return ("Port number must be between 1025 and 65535"); }
