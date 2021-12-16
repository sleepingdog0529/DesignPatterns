#include "event_dispatcher.h"
#include <iostream>

enum class Event{ Hey, Hello };

void Hey(const std::string& name)
{
	std::cout << "Hey " << name << "." << std::endl;
}

void Hello(const std::string& name)
{
	std::cout << "Hello " << name << "." << std::endl;
}

int main()
{
	using namespace wx2;

	EventDispatcher<Event, std::string> dispatcher;

	EventListener<Event, std::string> hey(Hey);
	EventListener<Event, std::string> hello(Hello);

	dispatcher.AddEventListener(Event::Hey, hey);
	dispatcher.AddEventListener(Event::Hello, hello);

	dispatcher.Dispatch(Event::Hey, "bob");
	dispatcher.Dispatch(Event::Hello, "tom");

	return 0;
}
