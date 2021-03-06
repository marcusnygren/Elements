#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
// Include GLEW
#include <GL/glew.h>

class Program
{

public:
	Program(const std::string name, const GLuint program, const std::string files);
	~Program();

	std::string getName() const;
	std::string getFiles() const;
	GLuint getProgram() const;

private:
	std::string _name;
	std::string _files;
	GLuint _program;

};


#endif // PROGRAM_H