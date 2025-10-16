#pragma once

#include <iostream>

#define BUFFER_SIZE 512

class Message
{
	public:
		Message();
		Message(std::string	&txt);
		Message(const Message &obj);
		~Message();
		Message &operator=(const Message &obj);

		void	parsMessage(std::string	message);
		void	getMessage();

	private:
		std::string		_buffer;
};
