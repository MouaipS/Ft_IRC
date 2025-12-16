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
		int			getFd() const;
		std::string getUsername() const;
		std::string getNickname() const;
		bool 		getIsAuthed() const;
		std::string&	getBuffer();

		//Setter
		void setFd(int fd);
		void setNickname(std::string nickname);
		void setUsername(std::string Username);
		void setIsAuthed(bool value);

		// Verification
		bool isUsernameValid(std::string value);
		bool isNicknameValid(std::string value);
		
	private :

		int			_fd;
		bool _isAuthed;
		std::string _username;
		std::string _nickname;
		std::string _buffer;
		int			_wallet;
};
