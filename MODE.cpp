#include "MODE.hpp"

Mode::Mode(int flag) : _flag(flag) {}

/**
 * @brief · i: Set/remove Invite-only channel
 */
void exec_i(Channel &channel) {
	if(channel.changeInvite()){
		std::cout << "REPONSE CHANGEMENT INVITE = actif" << std::endl;
	} else {
		std::cout << "REPONSE CHANGEMENT INVITE = inactif" << std::endl;
	}
}

/**
 * @brief · k: Set/remove the channel key (password)
 */
void exec_t(Channel &channel) {
	
	if(channel.changeKey()){
		std::cout << "REPONSE CHANGEMENT KEY = actif" << std::endl;
	} else {
		std::cout << "REPONSE CHANGEMENT KEY = inactif" << std::endl;
	}
}

void exec_k() {

}

void exec_o() {

}

void exec_l() {
	
}

void Mode::execCmd(User &user, Channel &channel) {
	std::string name = user.getUsername();
	std::vector<std::string>::iterator it = std::find(channel.getOperator().begin(), channel.getOperator().end(), name);
	if(it == channel.getOperator().end()) {
		std::cout << "PAS OPERATOR" << std::endl;
		return;
	} else {
		switch (_flag)
		{
		case(0) :
			exec_i(channel);
			break;
		case(1) :
			exec_t(channel);
			break;
		case(2) :
			exec_k();
			break;
		case(3) :
			exec_o();
			break;
		case(4) :
			exec_l();
		}
}