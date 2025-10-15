#pragma once
#include "iostream"
#include <vector>

class Channel {
	public :
		Channel(std::string name, std::string key, std::string topic);

		std::string getName() const;
		std::string getKey() const;
		void changeLimit(int limit);
		void setLimit();
		void setKey(std::string key);


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
