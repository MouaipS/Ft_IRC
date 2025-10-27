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
		std::vector<User*>	getUsers() const;
		std::vector<User*>	getOperators() const;
		std::string 		getTopic() const;
		std::string			getKey() const;
		bool				getIsInviteOnly() const;
		bool				getIsTopicProtected() const;
		bool				getIsKeyProtected() const;
		bool				getIsLimit() const;
		size_t				getUserLimit() const;
		
		// SET
		void	setIsInviteOnly(bool value);
		void	setTopic(const std::string& value);
		void	setIsTopicProtected(bool value);
		void	setKey(const std::string& value);
		void	setIsKeyProtected(bool value);
		void	setIsLimit(bool value);
		void	setUserLimit(size_t value);

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
		size_t				_userLimit;
};
