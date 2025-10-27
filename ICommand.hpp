#pragma once
#include "Channel.hpp"
#include "User.hpp"
#include <map>

class ICommand {

	public:
		virtual ~ICommand();

	private :
		virtual std::map<std::vector<User>, std::string> execCmd (void* data) = 0;
		//recoit les arguments et doit les parser
};