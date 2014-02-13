#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <QtGui/QOpenGLContext>

class Shader
{

public:
	Shader(const std::string name, GLuint compiledShader);
	~Shader();	
	GLuint getCompiledShader(void) const;
	std::string getName(void) const;
 
private:
	std::string _name;
	GLuint _compiledShader;

};

#endif // SHADER_H