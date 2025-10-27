#pragma once
#include "iostream"
#include <vector>

class User;

class Channel {
	public :
		Channel(std::string name);
		~Channel();

		std::string getName() const;
		std::string getTopic() const;
		std::vector<User> &getUsers();
		std::vector<User> &getOperators();

		// Getters
		bool getIsInviteMode() const;
		std::string getTopic() const;
		bool getIsRestrictionTopic() const;
		std::string getKey() const;
		bool getIsKeyProtected() const;
		int getUserLimit() const;
		bool getIsLimit() const;
		
		// Setters
		void setIsInviteMode(bool value);
		void setTopic(const std::string& value);
		void setIsRestrictionTopic(bool value);
		void setKey(const std::string& value);
		void setIsKeyProtected(bool value);
		void setUserLimit(int value);
		void setIsLimit(bool value);
		
		class UserLimitChannel: public std::exception {
			public: const char* what() const throw(); };

	private :
		std::string _name;
		std::vector<User> _users; //liste des users presents dans le channel
		std::vector<User> _operator; //liste des operateur du channel
		
		bool _isInviteMode;
		std::string _topic;
		bool _isRestrictionTopic;
		std::string _key;
		bool _isKeyProtected;
		int _userLimit;
		bool _isLimit;
};
