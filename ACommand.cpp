#include "ACommand.hpp"

ACommand::ACommand() {}

void ACommand::execCmd(User &user, Channel &channel) {
	std::cout << "Command default exec function.";
}
