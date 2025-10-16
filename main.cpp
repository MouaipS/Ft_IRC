#include "ACommand.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "User.hpp"
// #include "Message.hpp"
#include <sstream>

int main (int argc, char **argv) {

	if (argc == 2)
	{
		Server	ircSERV = Server();
		ircSERV.initServer(argv[1]);
	}
}
