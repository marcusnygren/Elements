#include "Define.h"

// Include standard headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>

#include "Debug.h"
#include "Console.h"
#include "Loader.h"
#include "Simulation.h"

// For the linker
Console* Console::theOnlyInstance = NULL;
std::mutex Console::_instanceMutex;
std::recursive_mutex Console::_commandItemsMutex;

bool showFps = true;


int main( void )
{
	bool isRunning = true;

	/* Initialize Console
	***************************************************************/
	Debug* debug = new Debug();
	Console* console = Console::getInstance();

	console->addItem("exit", [&](std::string args)
	{
		isRunning = false;
		debug->setIsRunning(false);
		//exit(EXIT_SUCCESS);
	}, "void", "Exits the program.");

	console->addItem("stop",[&](std::string args)
	{
		debug->setIsRunning(false);
	}, "void", "Stops the thread running the console.");
	

	/* Initialize GLFW
	***************************************************************/
	if( !glfwInit() )
	{
		std::cout << "Error (LINE: " << __LINE__ << ") failed to initialize GLFW" << std::endl;
		return -1;
	}


	/* Initialize OpenGL
	***************************************************************/
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	#if FULLSCREEN == 1
		window = glfwCreateWindow( FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT, "Elements", glfwGetPrimaryMonitor(), NULL);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	#else
		window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "Elements", NULL, NULL);
	#endif


	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	/* Initialize GLEW
	***************************************************************/
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	double _time = glfwGetTime();
	int nrOfFrames = 0;
	float speed = 3;
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	Simulation sim(VOLUME_WIDTH,VOLUME_HEIGHT,VOLUME_DEPTH);
	glEnable(GL_TEXTURE_3D);

	std::stringstream titleStream;
	titleStream << VOLUME_WIDTH << "x" << VOLUME_HEIGHT << "x" << VOLUME_DEPTH << " = " << VOLUME_WIDTH * VOLUME_HEIGHT * VOLUME_DEPTH << " Cells";
	glfwSetWindowTitle(window, titleStream.str().c_str());

	while(isRunning && glfwWindowShouldClose(window) == 0)
	{
		// if (showFps)
		// {
		// 	nrOfFrames++;
		// 	if (glfwGetTime() - _time >= 1)
		// 	{
		// 		std::stringstream ss;
		// 		ss << "FPS: " << (int) (1000.f / (float) nrOfFrames);
		// 		glfwSetWindowTitle(window, ss.str().c_str());
		// 		nrOfFrames = 0;
		// 		_time += 1;
		// 	}
		// }

		if(glfwGetKey(window, 'A') == GLFW_PRESS)
		{
			sim.addToSourcePosition(glm::vec3(-1,0,0) * speed);
		}

		if(glfwGetKey(window, 'S') == GLFW_PRESS)
		{
			sim.addToSourcePosition(glm::vec3(0,-1,0) * speed);
		}

		if(glfwGetKey(window, 'D') == GLFW_PRESS)
		{
			sim.addToSourcePosition(glm::vec3(1,0,0) * speed);
		}

		if(glfwGetKey(window, 'W') == GLFW_PRESS)
		{
			sim.addToSourcePosition(glm::vec3(0,1,0) * speed);
		}

		sim.stepSimulation();

		glfwSwapBuffers(window);

		if(glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, 1);
		}

		glfwPollEvents();
	}

	/* Cleanup
	***************************************************************/
	// Close OpenGL window and terminate GLFW
	std::cout << "Cleanup" << std::endl;
	glfwTerminate();
	
	if(debug) 
	{
		debug->setIsRunning(false);
		delete debug;
	}

	if(console)
		console->release();

	return 0;
}

