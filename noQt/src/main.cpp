// Include standard headers
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>

#include "Debug.h"
#include "Console.h"

// For the linker
Console* Console::theOnlyInstance = NULL;
std::mutex Console::_instanceMutex;
std::recursive_mutex Console::_commandItemsMutex;


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
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}


	/* Initialize OpenGL
	***************************************************************/
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Elements", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	/* Initialize GLEW
	***************************************************************/
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


	/* Render Loop
	***************************************************************/
	while(isRunning && glfwWindowShouldClose(window) == 0)
	{
		// Swap buffers
		glfwSwapBuffers(window);
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

