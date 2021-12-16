#include "event_dispatcher.h"
#include <iostream>

enum class PrintEvent { Hello, HelloWorld };

void PrintHello()
{
	std::cout << "Hello";
}

void PrintWorld()
{
	std::cout << "World";
}

void PrintPriod()
{
	std::cout << "." << std::endl;
}

void PrintExclamation()
{
	std::cout << "!!" << std::endl;
}

int main()
{
	EventDispatcher<PrintEvent> dispatcher;

	EventListener<PrintEvent> hello(PrintHello);
	EventListener<PrintEvent> world(PrintWorld);
	EventListener<PrintEvent> priod(PrintPriod);
	EventListener<PrintEvent> excl(PrintExclamation);

	dispatcher.AppendListener(PrintEvent::Hello, hello);
	dispatcher.AppendListener(PrintEvent::Hello, priod);
	dispatcher.AppendListener(PrintEvent::HelloWorld, hello);
	dispatcher.AppendListener(PrintEvent::HelloWorld, world);
	dispatcher.AppendListener(PrintEvent::HelloWorld, priod);

	std::cout << "Dispatch PrintEvent::Hello" << std::endl;
	dispatcher.Dispatch(PrintEvent::Hello);

	std::cout << "Dispatch PrintEvent::HelloWorld" << std::endl;
	dispatcher.Dispatch(PrintEvent::HelloWorld);

	dispatcher.RemoveListener(PrintEvent::HelloWorld, priod);
	dispatcher.AppendListener(PrintEvent::HelloWorld, excl);

	std::cout << "Dispatch PrintEvent::HelloWorld" << std::endl;
	dispatcher.Dispatch(PrintEvent::HelloWorld);

	return 0;
}
