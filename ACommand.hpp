#pragma once
#include "Channel.hpp"
#include "User.hpp"

class ACommand {
	public :
		ACommand();
		std::string _name;
	private :
		virtual void execCmd (User &user, Channel &channel);
};