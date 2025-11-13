#pragma once
#include <exception>
#include <iostream>

enum ErrorCode {
	
	ERR_NOSUCHNICK = 401,
	ERR_NOSUCHCHANNEL = 403,
	ERR_CANNOTSENDTOCHAN = 404,
	ERR_TOOMANYCHANNEL = 405,
	ERR_NOTEXTTOSEND = 412,
	ERR_INPUTTOOLONG = 417,
	ERR_UNKNOWNCOMMAND = 421,
	ERR_NONICKNAMEGIVEN = 431,
	ERR_NICKNAMEINUSE = 433,
	ERR_USERNOTINCHANNEL = 441,
	ERR_NOTONCHANNEL = 442,
	ERR_USERONCHANNEL = 443,
	ERR_NOTREGISTERED = 451,
	ERR_NEEDMOREPARAMS = 461,
	ERR_ALREADYREGISTRED = 462,
	ERR_PASSWDMISMATCH = 464,
	ERR_CHANOPRIVSNEEDED = 482
};

class ExceptionCode: public std::exception
{
	public:
		ExceptionCode(ErrorCode id) throw();
		virtual ~ExceptionCode() throw();

		const char* what() const throw();
		std::string	to_string() const;
		int			getCode() const;
		
	 private:	
		ErrorCode		code;
		std::string 	message;
};
