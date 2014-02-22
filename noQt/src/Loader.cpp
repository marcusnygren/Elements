#include "Loader.h"

Loader::Loader()
{
	Console* console = Console::getInstance();
	console->quickAdd("shaders", this, &Loader::printAvailableShaders);
	console->quickAdd("programs", this, &Loader::printAvailablePrograms);
}


Loader::~Loader()
{
	for_each(_shaders.begin(), _shaders.end(), [](Shader* pShader)
	{
		if(pShader)
			delete pShader;
	});

	for_each(_programs.begin(), _programs.end(), [](Program* pProgram)
	{
		if(pProgram)
			delete pProgram;
	});
}


GLenum Loader::getType(const std::string name) const
{
	GLenum type = 0;

	try
	{
		std::string ending = name.substr(name.find('.'), name.length());  
		
		if(ending == VERTEX_SHADER_TYPE__)
			type = GL_VERTEX_SHADER;
		else if(ending == FRAGMENT_SHADER_TYPE__)
			type = GL_FRAGMENT_SHADER;
		else
			std::cout << "The type was not defined for: " << name << std::endl;
	}
	catch(...)
	{
		std::cout << "Error (LINE: " << __LINE__ << ") reading the type of: " << name << std::endl;
	}

	return type;
}


void Loader::loadShaders(const std::string file)
{
	std::ifstream input(file.c_str(), std::ifstream::in);

	if(input.good()) 
	{
		std::string line;
		std::string type;
		std::string dir;

		try
		{
			dir = file.substr(0, file.rfind('/')+1);   
		}
		catch(...)
		{
			dir = "";
		}

		while(getline(input, line))
		{
			loadShader(dir + line, getType(line));
		}
		
		input.close();
	} 
	else
	{
		std::cout << "Error (LINE: " << __LINE__ << ") loading: " << file << std::endl;
	}
}


void Loader::loadShader(const std::string file, const GLenum shaderType)
{
	std::string name;
	try
	{
		 name = file.substr(file.rfind('/')+1);   
	}
	catch(...)
	{
		name = file;
	}

	bool isAlreadyAdded = false;
	for_each(_shaders.begin(), _shaders.end(), [&name, &isAlreadyAdded](Shader* pshader)
	{
		if(name == pshader->getName())
			isAlreadyAdded = true;
	});

	if(isAlreadyAdded)
		return;

	// Create shader
	GLuint shader = glCreateShader(shaderType);

	// Load code from file
	std::string code = loadCode(file);
	
	// Set source code
	char const* source = code.c_str();
  glShaderSource(shader, 1, &source, NULL);
	
	// Compile shader
	glCompileShader(shader);

	// Check shader compile status
#if SHADERS_CHECK_COMPILE_STATUS__
	checkShaderCompileStatus(file, shader);
#endif // SHADERS_CHECK_COMPILE_STATUS__

	// Add to DB
	_shaders.push_back(new Shader(name, shader));
}


GLuint Loader::accessShader(const std::string name) const
{
	GLuint shader;
	for_each(_shaders.begin(), _shaders.end(), [&name, &shader](Shader* pshader)
	{
		if(name == pshader->getName())
			shader = pshader->getCompiledShader();
	});
	return shader;
}


std::string Loader::loadCode(const std::string file)
{
	std::string code = "";
	std::ifstream input(file.c_str(), std::ifstream::in);

	if(input.good()) 
	{
		std::string line;

		while(getline(input, line))
			code += "\n" + line;
		
		input.close();
	} 
	else
	{
		std::cout << "Error (LINE: " << __LINE__ <<") loading vertex-shader: " << file << std::endl;
	}
	
	assert(!code.empty() && "A file was empty.");
	//std::cout << "Code: " << std::endl;
	//std::cout << code << std::endl << std::endl << std::endl;
	return code;
}


void Loader::checkShaderCompileStatus(const std::string file, const GLuint shader) const
{
	GLint result = GL_FALSE;
  int infoLogLength;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

	infoLogLength = infoLogLength < 1 ? 1 : infoLogLength;
  char* message = new char[infoLogLength];
	glGetShaderInfoLog(shader, infoLogLength, NULL, message);

	std::cout << file << " compile status: " << message << std::endl;

	delete [] message;
}


void Loader::checkProgramCompileStatus(std::string name, const GLuint program) const
{

	GLint result = GL_FALSE;
  int infoLogLength;

	glGetProgramiv(program, GL_LINK_STATUS, &result);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
	
	//infoLogLength = infoLogLength < 1 ? 1 : infoLogLength;
  char* message = new char[infoLogLength];
	glGetProgramInfoLog(program, infoLogLength, NULL, message);

	std::cout << name << " link status: " << message << std::endl;

	delete [] message;
}


void Loader::loadProgram(const std::string name, const std::string path, const std::string files)
{
	std::cout << "loadProgram(" << name << ", " << path << ", " << files << std::endl;
	bool isAlreadyAdded = false;

	for_each(_programs.begin(), _programs.end(), [&](Program* pProgram)
	{
		if(name == pProgram->getName())
			isAlreadyAdded = true;
	});

	if(isAlreadyAdded || name.empty() || files.empty())
		return;

	GLuint program = glCreateProgram();

	std::istringstream is(files);
	std::string shader;

	while(is >> shader && !shader.empty())
	{
		//std::cout << "LINE: " << shader << std::endl;
		loadShader(path + shader, getType(shader));
		glAttachShader(program, accessShader(shader));
	}

  glLinkProgram(program);

	// Check program compile status
#if PROGRAMS_CHECK_COMPILE_STATUS__
	checkProgramCompileStatus(name, program);
#endif // PROGRAMS_CHECK_COMPILE_STATUS__

	if(!name.empty() && !files.empty()) 
	{
		//std::cout << "\nName: '" << name << "'" << std::endl;
		//std::cout << "Files: '" << files << "'" << std::endl; 
		_programs.push_back(new Program(name, program, files));
	}

}


void Loader::loadPrograms(const std::string file)
{
	std::string path;

	try
	{
		path = file.substr(0, file.rfind('/')+1);   
	}
	catch(...)
	{
		path = "";
	}

	std::ifstream input(file.c_str(), std::ifstream::in);

	if(input.good()) 
	{
		std::string line;

		while(getline(input, line))
		{
			std::istringstream is(line);
			std::string name;
			std::string files;
			
			is >> name;
			getline(is, files);

			loadProgram(name, path, files);
		}
	} 
	else
	{
		std::cout << "Error (LINE: " << __LINE__ << ") loading programs from: " << file << std::endl;
	}

	input.close();
}


GLuint Loader::accessProgram(std::string name) const
{
	GLuint program;
	bool found = false;

	for_each(_programs.begin(), _programs.end(), [&](Program* pProgram)
	{
		if(name == pProgram->getName())
		{
			found = true;
			program = pProgram->getProgram();
		}
	});

  assert(found && "A program was not found.");
	return program;
}


void Loader::printAvailableShaders() const
{
	for_each(_shaders.begin(), _shaders.end(), [](Shader* pShader)
	{
		std::cout << "\t" << pShader->getName() << std::endl;
	});
}


void Loader::printAvailablePrograms() const
{
	for_each(_programs.begin(), _programs.end(), [](Program* pProgram)
	{
		std::cout << "\t" << pProgram->getName() << " : " 
							<< pProgram->getFiles() << std::endl;
	});
}
