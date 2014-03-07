#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <iostream>
#include <atomic>
#include <thread>

#include <QObject>

#include "Parameters.h"

#if ELEMENTS_DEBUG__
	#include "Debug.h"
#endif // ELEMENTS_DEBUG__


class DataHandler : public QObject
{
Q_OBJECT

public:
	DataHandler();
	~DataHandler();
	
	Q_INVOKABLE void setNParticles(const unsigned int nParticles);
	Q_INVOKABLE int getNParticles() const;

private:
	std::atomic<int> _nParticles;

#if ELEMENTS_DEBUG__
	void addConsoleCommands();
#endif // ELEMENTS_DEBUG__

};

#endif // DATAHANDLER_H