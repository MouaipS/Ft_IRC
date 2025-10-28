#include "User.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

User::User() {
	_isAuthed = false;
}

User::~User() {}

std::string User::getUsername() const {
	return(_username);
}

std::string User::getNickname() const {
	return(_nickname);
}

bool User::getIsAuthed() const {
	return(_isAuthed);
}

void User::setIsAuthed() {
	_isAuthed = true;
}

void User::setNickname(std::string nickname) {
	_nickname = nickname;
}

void User::setUsername(std::string username) {
	_username = username;
}

bool isAlphaNum(std::string name) {
	for(int i=0; i != name.length(); i++) {
		if(!isalnum(name[i]))
			return(false);
	}
	return(true);
}

bool isCmdName(std::string name) {
	std::string arr[] = {"KICK", "JOIN", "PASS", "MODE", "INVITE", "PRIVMSG", "MSG", "NICK"};

	for(int i = 0; i != arr->length(); i++) {
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
