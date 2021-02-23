// Supress unused variable warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"


#ifndef LOCAL_MESSAGE_HPP_INCLUDED
#define LOCAL_MESSAGE_HPP_INCLUDED
#include <SFML/Window.hpp>
#include "const/eodata.hpp"

class ChatMessage
{
public:
    sf::Clock received;
    std::string sender, receiver;
    std::string message;
	ChatType type;

	ChatMessage(std::string sender, std::string receiver, std::string message, ChatType type) : sender(sender), receiver(receiver), message(message), type(type){
		received.restart();
    };
};

#endif // LOCAL_MESSAGE_HPP_INCLUDED
