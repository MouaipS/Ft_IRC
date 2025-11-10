#include "CmdTest.hpp"

CmdTest::CmdTest(std::string serverName) : ICommand::ICommand(serverName) {};

void CmdTest::execCmd(
    int fd_origin,
    std::vector<std::string>& cmd,
    const std::string& password,
    std::vector<Channel*>& allChannels,
    std::map<int, User*>& fdToUser
) {
	(void)cmd;
 	std::cout << "[TEST]" << std::endl;
    User* origin = fdToUser[fd_origin];
    origin->setNickname("TestUser");
    origin->setUsername("test");
    origin->setIsAuthed(true);
	ICommand *TestJoin = new CmdJoin("ircTEST");
	std::vector<std::string> cmdtest;
    cmdtest.push_back("JOIN");
    cmdtest.push_back("#testchannel");
	TestJoin->execCmd(fd_origin, cmdtest, password, allChannels, fdToUser);
	delete(TestJoin);
}