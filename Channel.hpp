#pragma once
#include "iostream"
#include <vector>

class User;

class Channel {
	private :

		struct BJPlayer
	{
    	User* user;
    	std::vector<int> hand;
    	bool stayed;
	};

		bool					_BJMode;
		bool					_BJActif;
	    std::vector<BJPlayer> 	_bjPlayers;

	public :
		Channel(std::string name);
		~Channel();

		// GET

		std::string				getName() const;
		std::vector<User*>		getUsers() const;
		std::vector<User*>		getOperators() const;
		std::string 			getTopic() const;
		std::string				getKey() const;
		bool					getIsInviteOnly() const;
		bool					getIsTopicProtected() const;
		bool					getIsKeyProtected() const;
		bool					getIsLimit() const;
		size_t					getUserLimit() const;
		std::vector<User*>&		getGuestList() ;

		bool					getBJMode() const;
		bool					getIsBJRun() const;
		bool					isBJPlayer(User *user) const;
		std::vector<BJPlayer>& 	getBJPlayers();

		
		
		// SET

		void	setIsInviteOnly(bool value);
		void	setTopic(const std::string& value);
		void	setIsTopicProtected(bool value);
		void	setKey(const std::string& value);
		void	setIsKeyProtected(bool value);
		void	setIsLimit(bool value);
		void	setUserLimit(size_t value);
		void	setNewUser(User* user);
		void	setNewOperator(User *user);
		void	deleteUser(User* user);

		void	setBJ();

		// Utils

		int		findUser(User& user);
		User 	*findUser(std::string name);
		User	*findNick(std::string name);
		int		findOperator(User& user);
		void	addUserToChannel(User& user);
		void	removeUserFromChannel(User& user);
		void	promoteUser(User &user);
		void	demoteUser(User& user);
		void	addGuest(User& user);

		void	startBJ();
		void	stopBJ();
		void	addBJPlayer(User *user);
	
		class ChannelUserLimitExceededException: public std::exception {
			public: const char* what() const throw();
		};

		class UserNotFoundInChannelException: public std::exception {
			public: const char* what() const throw();
		};

		class UserNotOperatorException: public std::exception {
			public: const char* what() const throw();
		};

		class UserAlreadyInChannelException: public std::exception {
			public: const char* what() const throw();
		};

		class UserAlreadyOperatorException: public std::exception {
			public: const char* what() const throw();
		};

	private :
		const std::string	_name;
		std::vector<User*>	_users;
		std::vector<User*>	_operators;
		std::string 		_topic;
		std::string 		_key;
		std::vector<User*>	_guestlist;
		bool				_isInviteOnly;
		bool 				_isTopicProtected;
		bool 				_isKeyProtected;
		bool 				_isUserLimit;
		size_t				_userLimit;
};
