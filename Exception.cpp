#include "Exception.hpp"
#include <sstream>

ExceptionCode::ExceptionCode(ErrorCode id) throw(): code(id) {

	switch(code)
	{
		case ERR_NOSUCHNICK:
			message = ":No such nick/channel";
			break ;
		case ERR_NOSUCHCHANNEL:
			message = ":No such channel";
			break ;
		case ERR_CANNOTSENDTOCHAN:
			message = ":Cannot send to channel";
			break ;
		case ERR_TOOMANYCHANNEL:
			message = ":No such channel";
			break ;
		case ERR_NOTEXTTOSEND:
			message = ":No text to send";
			break ;
		case ERR_INPUTTOOLONG:
			message = ":Input line was too long";
			break ;
		case ERR_UNKNOWNCOMMAND:
			message = ":Unknown command";
			break ;
		case ERR_NONICKNAMEGIVEN:
			message = ":No nickname given";
			break ;
		case ERR_NICKNAMEINUSE:
			message = ":Nickname is already in use";
			break ;
		case ERR_USERNOTINCHANNEL:
			message = ":They aren't on that channel";
			break ;
		case ERR_NOTONCHANNEL:
			message = ":You're not on that channel";
			break ;
		case ERR_USERONCHANNEL:
			message = ":is already on channel";
			break ;
		case ERR_NOTREGISTERED:
			message = ":You have not registered";
			break ;
		case ERR_NEEDMOREPARAMS:
			message = ":Not enough parameters";
			break ;
		case ERR_ALREADYREGISTRED:
			message = ":Unauthorized command (already registered)";
			break ;
		case ERR_PASSWDMISMATCH:
			message = ":Password incorrect";
			break ;
		case ERR_CHANOPRIVSNEEDED:
			message = ":You're no channel operator";
			break ;
		default:
			message = ":default message";
			break ;
	}

}

ExceptionCode::~ExceptionCode() throw() {}

std::string ExceptionCode::to_string() const{

    std::ostringstream oss;
    oss << static_cast<int>(code);
    return oss.str();
}

const char* ExceptionCode::what() const throw() {

	return (message.c_str());
}

int	ExceptionCode::getCode() const {

	return (static_cast<int>(code));
}
