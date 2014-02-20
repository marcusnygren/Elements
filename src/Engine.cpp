#include "Engine.h"


Engine::Engine()
{
	std::cout << "ENGINE" << std::endl;


#if ELEMENTS_DEBUG__
	_debug = new Debug();
	addConsoleCommands();
#endif // ELEMENTS_DEBUG__
	
}


Engine::~Engine()
{

#if ELEMENTS_DEBUG__
	if(_debug)
		delete _debug;
#endif // ELEMENTS_DEBUG__

	if(_timer)
		delete _timer;

}

void Engine::setData(DataHandler* data)
{
	_data = data;
}


#if ELEMENTS_DEBUG__
void Engine::addConsoleCommands()
{
	Console* console = Console::getInstance();


	console->addItem("exit", [&](std::string args)
	{
		_debug->setIsRunning(false);
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
	
		_window = window();
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

	// Connect QT Signals inorder to be able to use OpenGL
	connect(_window, SIGNAL(sceneGraphInitialized()), this, SLOT(initialize()), Qt::DirectConnection);
	connect(_window, SIGNAL(beforeSynchronizing()), this, SLOT(sync()), Qt::DirectConnection);
	connect(_window, SIGNAL(beforeRendering()), this, SLOT(paint()), Qt::DirectConnection);
	
	// Make sure QML don't clear what we paint.
	_window->setClearBeforeRendering(false);

	// Displays FPS in windowtitle.
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(showFPS()), Qt::DirectConnection);
	_timer->start(50);

	std::cout << "INIT DONE" << std::endl;
}


// OpenGL initialization
void Engine::initialize()
{
	std::cout << "INITA" << std::endl;
	
	std::cout << "New Program" << std::endl;
	t = 0;
	_program = new QOpenGLShaderProgram();
	_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShader);
	_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShader);
		
	_program->bindAttributeLocation("vertices", 0);
		
	_program->link();

	connect(_window->openglContext(), SIGNAL(aboutToBeDestroyed()),
              this, SLOT(cleanup()), Qt::DirectConnection);
}


// Before painting, get data from GUI.
void Engine::sync()
{
//	std::cout << "SYNC" << std::endl;
	_nParticlesLive = _data->getNParticles();
}


void Engine::paint()
{	
	 //std::cout << "PAINT" << std::endl;

	_program->bind();

	_program->enableAttributeArray(0);

	float values[] = {
        -1, -1,
        1, -1,
        -1, 1,
        1, 1
	};
	_program->setAttributeArray(0, GL_FLOAT, values, 2);
	
	
	_program->setUniformValue("t", (float)t);
	t = t +0.01;
	if(t > 1.0) t = 0;
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
	_window->update();
}


// Keeps the loop going.
void Engine::update() const
{
	//std::cout << "Update" << std::endl;
	//_window->update();
}


void Engine::calculateFPS()
{
	_lastTime = _currentTime;
	_currentTime = QTime::currentTime();
	_elapsedTime = (_currentTime.second()*1000 + _currentTime.msec()) - (_lastTime.second()*1000 + _lastTime.msec());
	if (_elapsedTime != 0)
		_fps = 1000 / _elapsedTime;
	else
		_fps = 0;
}


void Engine::showFPS()
{
	std::ostringstream fps;
	fps << "Elements [" << _fps << "]";
	_window->setTitle(fps.str().c_str());
}


// After painting.
void Engine::cleanup()
{
	std::cout << "CLEANUP" << std::endl;
}


// When closing the window. 
void Engine::close()
{
	std::cout << "CLOSE" << std::endl;
}


void Engine::keyPressEvent(QKeyEvent* keyEvent)
{
	std::cout << "Pressed: " << keyEvent->text().toUtf8().constData() << std::endl;
}	


void Engine::keyReleaseEvent(QKeyEvent* keyEvent)
{
	std::cout << "Released: " << keyEvent->text().toUtf8().constData() << std::endl;
}