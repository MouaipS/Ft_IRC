#pragma once
#include "iostream"
#include <vector>

#define LIMIT_USER 9999

class User;

class Channel {
	public :
		Channel(std::string name, std::string key, std::string topic);

		std::string getKey() const;
		std::string getName() const;
		std::vector<std::string> &getUsersname();
		void 	changeLimit(int limit);
		void 	setLimit();
		void 	setKey(std::string key);
		void	addUser(User &user);
		

		class UserLimitChannel: public std::exception {
			public: const char* what() const throw(); };

	private :
		std::string _name;
		std::vector<std::string> _usersname;
		std::vector<std::string> _banUser;
		std::vector<std::string> _operator;
		
		int _limit;
		bool _isLimit;


		std::string _key; 
		bool _isKey;

		std::string _topic;
};
