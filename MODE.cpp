#include "MODE.hpp"

Mode::Mode(int flag) : _flag(flag) {}


void exec_i() {}
void exec_t() {}
void exec_k() {}
void exec_o() {}
void exec_l() {}

void Mode::execCmd(User &user, Channel &channel) {
	switch (_flag)
	{
	case(0) :
		exec_i();
		break;
	case(1) :
		exec_t();
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