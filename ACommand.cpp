#include "ACommand.hpp"

ACommand::ACommand() {}

void ACommand::execCmd(User &user, Channel &channel) {

	(void)user;
	(void)channel;
	std::cout << "Command default exec function.";
}
