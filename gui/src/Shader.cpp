#include "Shader.h"	

Shader::Shader(const std::string name, GLuint compiledShader)
	: _name(name)
	, _compiledShader(compiledShader)
{
}


Shader::~Shader()
{
	glDeleteShader(_compiledShader);
}


GLuint Shader::getCompiledShader(void) const
{
	return _compiledShader;
}


std::string Shader::getName(void) const
{
	return _name;
}