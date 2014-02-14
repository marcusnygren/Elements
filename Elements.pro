# Info
TARGET = Elements
DESTDIR = bin
OBJECTS_DIR = debug
MOC_DIR = moc
QT += qml quick widgets
CONFIG += console
CONFIG += c++11

# Input
HEADERS += src\Engine.h \
		   src\Parameters.h \
		   src\Console.h \
		   src\Debug.h \
		   src\DataHandler.h \
		   src\openglwindow.h

SOURCES += src\main.cpp \
		   src\Engine.cpp \
		   src\Debug.cpp \
		   src\DataHandler.cpp \
		   src\openglwindow.cpp
		  
RESOURCES += resources\GUI.qrc
