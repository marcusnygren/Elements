TARGET = Elements
DESTDIR = bin
OBJECTS_DIR = debug
MOC_DIR = moc
QT += qml quick

# Input
HEADERS += src\squircle.h
SOURCES += src\squircle.cpp src\main.cpp
RESOURCES += resources\GUI.qrc