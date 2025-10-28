#include "ICommand.hpp"
#include "Channel.hpp"
#include "Server.hpp"
#include "User.hpp"
#include <sstream>

int main (int argc, char **argv) {

	if (argc != 2)
	{
		std::cout << "Bad arguments : " << argv[0] << " <PORT> <PASSWORD>" << std::endl;
		return (2);
	}

	Server	ircSERV = Server(argv[1], argv[2]);
}
