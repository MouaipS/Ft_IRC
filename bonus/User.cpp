#include "User.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

User::User() :  _wallet(100), _isAuthed(false){
	(void)_wallet;
}

User::~User() {}

std::string User::getUsername() const {
	return(_username);
}

std::string User::getNickname() const {
	return(_nickname);
}

std::string& User::getBuffer()
{
	return (_buffer);
}

int	User::getFd() const {

	return (_fd);
}

bool User::getIsAuthed() const {
	return(_isAuthed);
}

void User::setIsAuthed(bool value) {
	_isAuthed = value;
}

void User::setNickname(std::string nickname) {
	_nickname = nickname;
}

void User::setUsername(std::string username) {
	_username = username;
}

void User::setFd(int fd) {

	_fd = fd;
}

bool isAlphaNum(std::string name) {
	for(size_t i=0; i != name.length(); i++) {
		if(!isalnum(name[i]))
			return(false);
	}
	return(true);
}

bool isCmdName(std::string name) {
	std::string arr[8] = {"KICK", "JOIN", "PASS", "MODE", "INVITE", "PRIVMSG", "MSG", "NICK"};

	for(size_t i = 0; i != 8; i++) {
		if(arr[i] == name)
			return(false);
	}
	return (true);
}

bool User::isUsernameValid(std::string name) {
	if(name.empty() || !isAlphaNum(name) || !isCmdName(name)) {return false;}
	return true;
}

bool User::isNicknameValid(std::string name) {
	if(name.empty() || !isAlphaNum(name) || !isCmdName(name)) {return false;}
	return true;
}
