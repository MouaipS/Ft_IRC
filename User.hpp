#pragma once 
#include "iomanip"
#include "Channel.hpp"
#include <list>
#include "cstdlib"

class User {
	public :
		User(std::string username, std::string nickname, Channel home);
			~User();

		//Geter
		std::string getUsername() const;
		std::string getNickname() const;
		std::vector<std::string> getChannel() const;

		//Setter
		void ChangeNickname(std::string nickname);
		
	private :
		const std::string _username;
		std::string _nickname;
		std::list<Channel> _MyChannels;
};

std::ostream &operator<<(std::ostream &os, const User &User);
