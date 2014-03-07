#ifndef ENGINE_H
#define ENGINE_H

#include <iostream>
#include <cassert>
#include <istream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>

#include <QtOpenGL>
#include <QtGui>
#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtQuick/qquickwindow.h>
#include <QtGui/QOpenGLContext>
#include <QTime>
#include <QTimer>
#include <QOpenGLFunctions>
#include <QQmlContext>
#include <QQmlEngine>
#include <QSurfaceFormat>
#include <QSurface>
#include <QKeyEvent>
#include <QOpenGLFunctions_3_3_Core>
#include <QtGui/QOpenGLFunctions_3_3_Core>
#include <QtGui/QOpenGLVersionFunctions>
#include <QOpenGLFunctions_3_3_Compatibility>
// #include <QAbstractOpenGLFunctions>

#include "Parameters.h"
#include "DataHandler.h"

#if ELEMENTS_DEBUG__
	#include "Debug.h"
#endif // ELEMENTS_DEBUG__


class Engine : public QQuickItem //, protected QOpenGLFunctions_3_3_Core
{
Q_OBJECT
	
public:
	Engine();
	~Engine();

	Q_INVOKABLE void setData(DataHandler* data);
	Q_INVOKABLE void update() const;

public slots:
	void initialize();
	void sync();
	void paint();
	void cleanup();
	void showFPS();

protected:
	void itemChange(ItemChange change, const ItemChangeData&);
	void keyPressEvent(QKeyEvent* keyEvent);
	void keyReleaseEvent(QKeyEvent* keyEvent);
	

void createVolume(uint width, uint height, uint depth, uint nrComponents);
	virtual bool initializeOpenGLFunctions() 
	{
		return true;
	}

private:
	void init();
	void close();
	void calculateFPS();

	QQuickWindow* _window;
	DataHandler* _data;


	int _nParticlesLive;

	QSurfaceFormat* _format;
	QOpenGLFunctions * _GLFunctions;
	QOpenGLContext* _GUIContext;        
	QOpenGLContext* _GLContext;
	QOpenGLShaderProgram* _program;

	float t;


	// For showing FPS
	QTimer* _timer;

	// For calculating FPS
	QTime _currentTime;
	QTime _lastTime;
	int _elapsedTime;
	float _fps;


#if ELEMENTS_DEBUG__
	void addConsoleCommands();
	Debug* _debug;
#endif // ELEMENTS_DEBUG__

};

#endif // ENGINE_H