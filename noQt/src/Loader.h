#ifndef LOADER_H
#define LOADER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
// Include GLEW
#include <GL/glew.h>


#include "Shader.h"
#include "Program.h"
#include "Console.h"

// The following should be after Parameters.h

// To determine the shader filetype (string)
#ifndef VERTEX_SHADER_TYPE__
	#define VERTEX_SHADER_TYPE__ ".vertex"
#endif // VERTEX_SHADER_TYPE__

#ifndef FRAGMENT_SHADER_TYPE__
	#define FRAGMENT_SHADER_TYPE__ ".frag"
#endif // FRAGMENT_SHADER_TYPE__

// For checking shader compile status (0 / 1)
#ifndef SHADERS_CHECK_COMPILE_STATUS__
	#define SHADERS_CHECK_COMPILE_STATUS__ 1
#endif // SHADERS_CHECK_COMPILE_STATUS__

// For checking program compile status (0 / 1)
#ifndef PROGRAMS_CHECK_COMPILE_STATUS__
	#define PROGRAMS_CHECK_COMPILE_STATUS__ 1
#endif // PROGRAMS_CHECK_COMPILE_STATUS__


class Loader 
{

public:
	Loader();
	~Loader();
		
	std::string loadCode(const std::string file);
	GLenum getType(const std::string name) const;

	void loadShader(const std::string file, const GLenum shaderType);
	void loadShaders(const std::string file);
	GLuint accessShader(const std::string name) const; 

	void loadProgram(const std::string name, const std::string path, const std::string files);
	void loadPrograms(const std::string file);
	GLuint accessProgram(std::string name) const;

	void printAvailableShaders() const;
	void printAvailablePrograms() const;
	
private:
	void checkShaderCompileStatus(const std::string file, const GLuint shader) const;
	void checkProgramCompileStatus(std::string name, const GLuint program) const;

	std::vector<Shader*> _shaders;
	std::vector<Program*> _programs;
	
};

#endif // LOADER_H