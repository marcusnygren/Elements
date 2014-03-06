#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <list>
#include <algorithm>

#include "Parameters.h"
#include "Console.h"

class Debug
{

public:
	Debug();
	~Debug();
	void setIsRunning(bool isRunning);
	bool getIsRunning();

private:
	void consoleSetUp();
	void runConsole();

	std::thread _consoleThread;
	bool _isRunning;
	std::mutex _isRunningMutex;

};

#endif // DEBUG_H

