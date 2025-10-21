#pragma once 
#include "ACommand.hpp"

class Mode : public ACommand {
	public :
		Mode(int flag);
		int _flag;
	private :
		void execCmd (User &user, Channel &channel);
};