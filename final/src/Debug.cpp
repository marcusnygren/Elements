#include "Debug.h"

Debug::Debug()
{
	consoleSetUp();
}


Debug::~Debug()
{
	_consoleThread.join();
	Console::getInstance()->release();
}


void Debug::consoleSetUp()
{
	setIsRunning(true);
	std::thread consoleThread(&Debug::runConsole, this);
	_consoleThread = std::move(consoleThread);
}


void Debug::setIsRunning(bool isRunning)
{
	std::unique_lock<std::mutex> lock(_isRunningMutex);
	_isRunning = isRunning;
}


bool Debug::getIsRunning()
{
	std::unique_lock<std::mutex> lock(_isRunningMutex);
	return _isRunning;
}


void Debug::runConsole()
{
	std::string command;
	while(getIsRunning()) 
	{
		getline(std::cin, command);
		Console::getInstance()->executeCommand(command);
	}
	std::cout << "Console stopped running." << std::endl;
}