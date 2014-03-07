#include "DataHandler.h"

DataHandler::DataHandler()
	: _nParticles(1)
{
#if ELEMENTS_DEBUG__
	addConsoleCommands();
#endif // ELEMENTS_DEBUG__
}


DataHandler::~DataHandler()
{
}


#if ELEMENTS_DEBUG__
void DataHandler::addConsoleCommands()
{
	Console* console = Console::getInstance();
	
	console->quickAdd("setNParticles", this, &DataHandler::setNParticles);
	console->addItem("getNParticles", [&](std::string)
	{
		std::cout << "_nParticles = " << _nParticles << std::endl;
	});

}
#endif // ELEMENTS_DEBUG__


void DataHandler::setNParticles(const unsigned int nParticles)
{
	_nParticles = nParticles;
}


int DataHandler::getNParticles() const
{
	return _nParticles;
}


