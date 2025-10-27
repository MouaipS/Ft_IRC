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
		bool 		getIsAuthed() const;

		//Setter
		void setNickname(std::string nickname);
		void setUsername(std::string Username);
		void setIsAuthed();

		// Verification
		bool isUsernameValid(std::string value);
		bool isNicknameValid(std::string value);
		
	private :

		bool _isAuthed;
		std::string _username;
		std::string _nickname;
};