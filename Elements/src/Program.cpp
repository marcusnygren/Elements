#include "Program.h"

Program::Program(const std::string name, const GLuint program, const std::string files)
	: _name(name)
	, _program(program)
	, _files(files)
{

}


Program::~Program()
{
	glDeleteProgram(_program);
}


std::string Program::getName() const
{
	return _name;
}


std::string Program::getFiles() const
{
	return _files;
}

GLuint Program::getProgram() const
{
	return _program;
}