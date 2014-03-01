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
	window = glfwCreateWindow( 300, 300, "Elements", NULL, NULL);
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
	Simulation test(100,100,100,0.1);
	 glEnable(GL_TEXTURE_3D);
	while(isRunning && glfwWindowShouldClose(window) == 0)
	{
		if (showFps)
		{
			nrOfFrames++;
			if (glfwGetTime() - _time >= 1)
			{
				std::stringstream ss;
				ss << "FPS: " << (int) (1000.f / (float) nrOfFrames);
				glfwSetWindowTitle(window, ss.str().c_str());
				nrOfFrames = 0;
				_time += 1;
			}
		}

		if(glfwGetKey(window, 'A') == GLFW_PRESS)
		{
			test.addToSourcePosition(glm::vec3(-1,0,0) * speed);
		}

		if(glfwGetKey(window, 'S') == GLFW_PRESS)
		{
			test.addToSourcePosition(glm::vec3(0,-1,0) * speed);
		}

		if(glfwGetKey(window, 'D') == GLFW_PRESS)
		{
			test.addToSourcePosition(glm::vec3(1,0,0) * speed);
		}

		if(glfwGetKey(window, 'W') == GLFW_PRESS)
		{
			test.addToSourcePosition(glm::vec3(0,1,0) * speed);
		}

		test.stepSimulation();
		// test.render();
		glfwSwapBuffers(window);

		glfwPollEvents();

	}

	// // Dark blue background
	// glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	// glViewport(0, 0, 1024, 768);

	// GLuint VertexArrayID;
	// glGenVertexArrays(1, &VertexArrayID);
	// glBindVertexArray(VertexArrayID);

	// static const GLfloat g_vertex_buffer_data[] = { 
	// 	-1.0f, -1.0f, 0.0f,
	// 	 1.0f, -1.0f, 0.0f,
	// 	 0.0f,  1.0f, 0.0f,
	// };

	// GLuint vertexbuffer;
	// glGenBuffers(1, &vertexbuffer);
	// glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	// Loader loader;
	// loader.loadPrograms("shaders/programs.txt");


	/* Render Loop
	***************************************************************/
	// while(isRunning && glfwWindowShouldClose(window) == 0)
	// {
	// 	// Clear the screen
	// 	glClear( GL_COLOR_BUFFER_BIT );

	// 	// Use our shader
	// 	glUseProgram(loader.accessProgram("program1"));

	// 	// 1rst attribute buffer : vertices
	// 	glEnableVertexAttribArray(0);
	// 	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	// 	glVertexAttribPointer(
	// 		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
	// 		3,                  // size
	// 		GL_FLOAT,           // type
	// 		GL_FALSE,           // normalized?
	// 		0,                  // stride
	// 		(void*)0            // array buffer offset
	// 	);

	// 	// Draw the triangle !
	// 	glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

	// 	glDisableVertexAttribArray(0);

	// 	// Swap buffers
	// 	glfwSwapBuffers(window);
	// 	glfwPollEvents();
	// } 


	/* Cleanup
	***************************************************************/
	// Close OpenGL window and terminate GLFW
	std::cout << "Cleanup" << std::endl;
	glfwTerminate();

	// glDeleteBuffers(1, &vertexbuffer);
	// glDeleteVertexArrays(1, &VertexArrayID);
	
	if(debug) 
	{
		debug->setIsRunning(false);
		delete debug;
	}

	if(console)
		console->release();

	return 0;
}

