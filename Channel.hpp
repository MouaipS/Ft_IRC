#pragma once
#include "iostream"
#include <vector>

class User;

class Channel {
	public :
		Channel(std::string name);
		~Channel();

		// GET
		std::string			getName() const;
		std::vector<User>	&getUsers();
		std::vector<User>	&getOperators();
		std::string 		getTopic() const;
		std::string			getKey() const;
		bool				getIsInviteOnly() const;
		bool				getIsTopicProtected() const;
		bool				getIsKeyProtected() const;
		bool				getIsLimit() const;
		int					getUserLimit() const;
		
		// SET
		void	setIsInviteOnly(bool value);
		void	setTopic(const std::string& value);
		void	setIsTopicProtected(bool value);
		void	setKey(const std::string& value);
		void	setIsKeyProtected(bool value);
		void	setUserLimit(int value);
		void	setIsLimit(bool value);

		// Utils
		size_t	findUser(User& user);
		size_t	findOperator(User& user);
		bool	addUserToChannel(User& user);
		bool	removeUserFromChannel(User& user);
		bool	promoteUser(User& user);
		bool	demoteUser(User& user);
		
		class UserLimitChannel: public std::exception {
			public: const char* what() const throw(); };

	private :
		const std::string	_name;
		std::vector<User*>	_users;
		std::vector<User*>	_operators;
		std::string 		_topic;
		std::string 		_key;
		bool				_isInviteOnly;
		bool 				_isTopicProtected;
		bool 				_isKeyProtected;
		bool 				_isUserLimit;
		int					_userLimit;
};
