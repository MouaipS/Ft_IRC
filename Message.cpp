#include "Message.hpp"

Message::Message() {}

Message::Message(const Message &obj) {

	if (this != &obj) {
		Message::operator=(obj);
	}
}

Message::Message(std::string &txt) {

	_buffer.assign(txt, 0, BUFFER_SIZE);
	txt.erase(0, BUFFER_SIZE);
	std::cout << _buffer.size() << " # " << _buffer << std::endl;
}

Message::~Message() {}

Message	&Message::operator=(const Message &obj) {

	if (this != &obj) {
		_buffer = obj._buffer;
	}
	return (*this);
}
