#include "Server.hpp"
#include "ICommand.hpp"
#include <vector>

Server::Server(std::string port, std::string password): _port(port), _password(password) {}

Server::~Server() {
	
	std::map<std::string, ICommand*>::iterator it = _commands.begin();
	for (;it != _commands.end(); ++it)
		delete it->second;
}

// F U N C T I O N S

/**
 * @brief Accepts a new client connection and registers it with epoll.
 *
 * This function is triggered when the main server socket (`_sockfd`)
 * receives a new connection request. It accepts the client, makes its
 * file descriptor non-blocking, and adds it to the epoll monitoring set.
 * A new `User` instance is created and stored in the `_fdToUser` map.
 *
 * @param fd_actif File descriptor of the active socket (the server socket).
 * @param dataEpoll Structure used to register new epoll events.
 * @param epoll_fd File descriptor of the epoll instance.
 */
void Server::NewClient(int fd_actif, epoll_event dataEpoll, int epoll_fd) {
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

/**
 * @brief Initializes the command list for the server.
 *
 * Creates and registers all supported IRC commands (KICK, INVITE, TOPIC, etc.)
 * Each command is stored as a pair of command name and its corresponding
 * `ICommand`-derived object in the `_commands` map.
 *
 * @note Memory for command objects is allocated dynamically and freed
 * in the destructor.
 */
void	Server::initCommands() {

	_commands.insert(std::make_pair("KICK", new CmdKick(SERVERNAME)));
	std::cout << _commands.begin()->first << std::endl;
	if (!_commands.begin()->second)
		throw InitCommandFail();
	_commands.insert(std::make_pair("INVITE", new CmdInvite(SERVERNAME)));
	std::cout << _commands.begin()->first << std::endl;
	if (!_commands.begin()->second)
		throw InitCommandFail();
	_commands.insert(std::make_pair("AAA", new CmdTopic(SERVERNAME)));
	std::cout << _commands.begin()->first << std::endl;
	if (!_commands.begin()->second)
		throw InitCommandFail();
	_commands.insert(std::make_pair("MODE", new CmdMode(SERVERNAME)));
	std::cout << _commands.begin()->first << std::endl;
	if (!_commands.begin()->second)
		throw InitCommandFail();
	_commands.insert(std::make_pair("JOIN", new CmdJoin(SERVERNAME)));
	if (!_commands.begin()->second)
		throw InitCommandFail();
	_commands.insert(std::make_pair("NICK", new CmdNick(SERVERNAME)));
	if (!_commands.begin()->second)
		throw InitCommandFail();
	_commands.insert(std::make_pair("PASS", new CmdPass(SERVERNAME)));
	if (!_commands.begin()->second)
		throw InitCommandFail();
	_commands.insert(std::make_pair("PRIVMSG", new CmdPrivmsg(SERVERNAME)));
	if (!_commands.begin()->second)
		throw InitCommandFail();
	_commands.insert(std::make_pair("USER", new CmdUser(SERVERNAME)));
	if (!_commands.begin()->second)
		throw InitCommandFail();
}

/**
 * @brief Sets up the main server socket and starts listening for connections.
 *
 * This function configures address information, creates a socket, binds it
 * to the given port, and starts listening for incoming client connections.
 *
 * @param port The port number (as string) on which the server will listen.
 * @throw GetAddrInfoFail If `getaddrinfo()` fails.
 * @throw SocketFail If socket creation fails.
 * @throw BindFail If the socket binding fails.
 * @throw ListenFail If the `listen()` system call fails.
 */
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

/**
 * @brief Checks if the user's input buffer contains a complete command.
 *
 * Determines if the buffer includes the IRC message delimiter `\r\n`
 * and ensures the total command length does not exceed 510 characters.
 *
 * @param buffer Reference to the user's input buffer.
 * @return true if a complete and valid command is ready to process.
 * @return false otherwise.
 */
bool Server::isBufferReady(std::string& buffer)
{
	size_t	pos = buffer.find("\r\n");

	if (pos != std::string::npos && pos <= 510)
		return (true);

	return (false);
}

/**
 * @brief Handles situations where the client's input exceeds the allowed length.
 *
 * When a user sends a message longer than the IRC protocol limit (510 chars),
 * this function clears their buffer and sends them an error message.
 *
 * @param fd File descriptor of the client socket.
 * @param user Pointer to the associated `User` object.
 * @param buffer Reference to the user's buffer (will be cleared).
 */
void Server::handleBufferTooLong(int fd, User *user, std::string& buffer)
{
	if (!user)
		return ;

	buffer.clear();
	std::string message = ":";
	message = message + SERVERNAME + " 417 " + user->getNickname() + " :Input line was too long\r\n";
	send(fd, message.c_str(), message.length(), 0);
}

/**
 * @brief Handles an epoll event (either a new client or client activity).
 *
 * If the event corresponds to the server socket, it accepts a new client.
 * Otherwise, it reads data from the existing client's socket, updates
 * their buffer, checks for complete commands, and dispatches them to the
 * corresponding command handler.
 *
 * @param event The triggered epoll event (either new connection or data ready).
 * @param dataEpoll The epoll event template for adding new clients.
 * @param epoll_fd The epoll instance file descriptor.
 */
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
		sendToCommand(args, fd_actif);
		userBuffer.clear();
	} 
	else if (userBuffer.size() > 510)
		handleBufferTooLong(fd_actif, user, userBuffer);
}

/**
 * @brief Main event loop of the server using epoll.
 *
 * Continuously waits for network events using `epoll_wait()` and
 * dispatches them to `handle_event()` for processing.
 * This function effectively keeps the server running.
 *
 * @note This loop runs indefinitely until the process is stopped.
 */
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

/**
 * @brief Reads incoming data from a client socket and updates their buffer.
 *
 * If data is received, it is appended to the user’s input buffer.
 * If the client disconnects (`recv` returns 0), their `User` object
 * is deleted and removed from `_fdToUser`.
 *
 * @param fd_actif The client’s file descriptor.
 * @param user Pointer to the `User` object associated with the client.
 */
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

/**
 * @brief Executes a command received from a client.
 *
 * Finds the corresponding `ICommand` object from the `_commands` map
 * and executes it via its `execCmd()` method.
 * If the command is unknown, sends an error message back to the client.
 *
 * @param cmd Vector of parsed command arguments.
 * @param fd_origin File descriptor of the client that issued the command.
 */
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

const char* Server::InitCommandFail::what() const throw() {
	return ("Init command failed"); }
