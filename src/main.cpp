#include <QGuiApplication>
#include <QtQuick/QQuickView>

#include "Parameters.h"
#include "Engine.h"
#include "DataHandler.h"
#include "Debug.h" 

#include <QSurfaceFormat>


int main(int argc, char** argv) 
{
	QGuiApplication app(argc, argv);

	// Make C++ class available to QML. 
	// Uniform resource identifier: Elements
	// XML version: 1.0
	// Name: Engine 
	qmlRegisterType<Engine>("ElementsEngine", 1, 0, "Engine");
	qmlRegisterType<DataHandler>("ElementsDataHandler", 1, 0, "Data");

	// QQuickView view(QUrl("qrc:///Elements/GUI.qml"), QWindow * parent = 0);
	QQuickView view;
  view.setSource(QUrl("qrc:///Elements/GUI.qml"));
	view.show();
	
	// Returns a infinite loop which looks for events/signals.
	// It will exit the loop when exit() is called.
	return app.exec();
}

// For the linker
Console* Console::theOnlyInstance = NULL;
std::mutex Console::_instanceMutex;
std::recursive_mutex Console::_commandItemsMutex;

