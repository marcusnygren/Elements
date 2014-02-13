#include "Engine.h"
#include <QQmlContext>
#include <QQmlEngine>
#include <QSurfaceFormat>
#include <QSurface>



Engine::Engine()
{
	std::cout << "ENGINE" << std::endl;


#if ELEMENTS_DEBUG__
	_debug = new Debug();
	addConsoleCommands();
#endif // ELEMENTS_DEBUG__
	

	//_data = new DataHandler();
}


Engine::~Engine()
{
	//close();

#if ELEMENTS_DEBUG__
	if(_debug)
		delete _debug;
#endif // ELEMENTS_DEBUG__

	// Placed after deleting _debug to make sure the Console 
	// can access the data while running.
//	if(_data)
	//	delete _data;
}


void Engine::setData(DataHandler* data)
{
	_data = data;
}

/*
DataHandler* Engine::getData(void) const
{
	return _data;
}
*/

#if ELEMENTS_DEBUG__
void Engine::addConsoleCommands()
{
	Console* console = Console::getInstance();


	console->addItem("exit", [](std::string args)
	{
		exit(EXIT_SUCCESS);
	}, "void", "Exits the program.");


	console->addItem("stop",[&](std::string args)
	{
		_debug->setIsRunning(false);
	}, "void", "Stops the thread running the console.");


	console->addItem("fps", [&](std::string args)
	{
		std::cout << "FPS: " << _fps << std::endl;
	}, "void", "Prints the current FPS.");

}
#endif // ELEMENTS_DEBUG__


void Engine::itemChange(ItemChange change, const ItemChangeData&)
{
	// The ItemSceneChange event is sent the window is attached 
	// and when closing the window (then will window() == NULL).
	if(change == ItemSceneChange) 
	{
		if(!window())
		{
			close();
			return;
		}
	
		_win = window();
		init();
	}
}

static char* vertexShader = "attribute highp vec4 vertices;"
                                           "varying highp vec2 coords;"
                                           "void main() {"
                                           "    gl_Position = vertices;"
                                           "    coords = vertices.xy;"
                                           "}\0";


static char* fragmentShader = "uniform lowp float t;"
                                           "varying highp vec2 coords;"
                                           "void main() {"
                                           "    lowp float i = 1. - (pow(coords.x, 4.) + pow(coords.y, 4.));"
                                           "    i = smoothstep(t - 0.8, t + 0.8, i);"
                                           "    i = floor(i * 20.) / 20.;"
                                           "    gl_FragColor = vec4(coords * .5 + .5, i, i);"
                                           "}\0";

// At start up.
void Engine::init()
{
		std::cout << "INIT" << std::endl;

/*
			std::cout << "INNN";
			QQmlEngine *en = QQmlEngine::contextForObject(this->findChild<QObject *>("engine"))->engine();
			 qDebug() << "\nENGINE:	baseUrl() = " << en->baseUrl();
			en->rootContext()->setContextProperty("data", _data);
			std::cout << "HEJ";
*/

	{
		std::condition_variable cv;
		std::mutex m;
		std::unique_lock<std::mutex> lk(m);
		cv.wait(lk, [this]{ 
				std::cout << "Waiting on data... (might be deadlock here :) )" << std::endl;
				return _data; 
		});
		std::cout << "GOT DATA SAFELY"<< std::endl;
	}

	connect(_win, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
	connect(_win, SIGNAL(beforeRendering()), this, SLOT(paint()), Qt::DirectConnection);
	
	// Make sure QML don't clear what we paint.
	_win->setClearBeforeRendering(false);


	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(QQuickView::update()), Qt::DirectConnection);
	_timer->start(16);




/*
	// http://qt-project.org/doc/qt-5.0/qtgui/qsurfaceformat.html#QSurfaceFormat
  _format = new QSurfaceFormat();
	_format->setRenderableType(QSurfaceFormat::RenderableType::OpenGL);
	_format->setMajorVersion(3);
	_format->setMinorVersion(3);
//_win->openglContext()->setFormat(*_format);


	// http://qt-project.org/doc/qt-5.0/qtgui/qopenglcontext.html#setFormat


  _context = new QOpenGLContext(this);
  _context->setFormat(*_format);
  _context->create(); // Can't find EGL config, returning NULL.
  _context->makeCurrent(_win);
	


	if(!_program) {
		std::cout << "New Program" << std::endl;
		_program = new QOpenGLShaderProgram();
		_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader);
		_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader);
		
		_program->bindAttributeLocation("vertices", 0);
		_program->link();

		connect(_win->openglContext(), SIGNAL(aboutToBeDestroyed()), this, SLOT(cleanup()), Qt::DirectConnection);

	}
*/

	std::cout << "init done" << std::endl;
}


// Before painting.
void Engine::sync()
{
	std::cout << "SYNC" << std::endl;
	_nParticlesLive = _data->getNParticles();
}


// During painting.
// http://www.kdab.com/opengl-in-qt-5-1-part-1/
// http://www.kdab.com/opengl-in-qt-5-1-part-2/
void Engine::paint()
{	
//	 std::cout << "PAINT" << std::endl;


if(!_program) {
		std::cout << "New Program" << std::endl;
		t = 0;
		_program = new QOpenGLShaderProgram();
		_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader);
		_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader);
		
		_program->bindAttributeLocation("vertices", 0);
		_program->link();

		connect(window()->openglContext(), SIGNAL(aboutToBeDestroyed()),
                this, SLOT(cleanup()), Qt::DirectConnection);

	}
	_program->bind();

	_program->enableAttributeArray(0);

	float values[] = {
        -1, -1,
        1, -1,
        -1, 1,
        1, 1
	};
	_program->setAttributeArray(0, GL_FLOAT, values, 2);
	
	
		_program->setUniformValue("t", (float)1.0/_fps);
	t = t +0.01;
	glViewport(0, 0, window()->width(), window()->height());

	glDisable(GL_DEPTH_TEST);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	_program->disableAttributeArray(0);
	_program->release();

	calculateFPS();
}


void Engine::calculateFPS()
{
	_lastTime = _currentTime;
	_currentTime = QTime::currentTime();
	_elapsedTime = (_currentTime.second()*1000 + _currentTime.msec()) - (_lastTime.second()*1000 + _lastTime.msec());
	_fps = 1000 / _elapsedTime;
	std::cout << _fps << std::endl;

	std::ostringstream fps;
	fps << "Elements [" << _fps << "]";
	_win->setTitle(fps.str().c_str());
}

// After painting.
void Engine::cleanup()
{
	std::cout << "CLEANUP" << std::endl;
}


// When closing the window / ~Engine() is called.
void Engine::close()
{
	std::cout << "CLOSE" << std::endl;
}