#pragma once 
#include "iomanip"
#include "Channel.hpp"
#include <list>
#include "cstdlib"

class User {

	public :
			User();
			~User();

		//Geter
		std::string getUsername() const;
		std::string getNickname() const;

		//Setter
		void setNickname(std::string nickname);
		void setUsername(std::string Username);

		// Verification
		bool isNicknameValid(std::string value);
		bool isUsernameValid(std::string value);
		
	private :

		bool isAuthed;
		std::string _username;
		std::string _nickname;
};