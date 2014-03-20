#include "Define.h"
#include "Simulation.h"

#include <iostream>
Simulation::Simulation(int width, int height, int depth)
: _density(width, height, depth, 3),
  _velocity(width, height, depth, 3),
  _pressure(width, height, depth, 2),
  _divergence(width, height, depth, 2),
  _obstacles(width, height, depth, 3),
  _temperature(width, height, depth, 2),
  _timeStep(TIME_STEP),
  _gridScale(1.0f/(float)width, 1.0f/ (float)height, 1.0f/(float) depth),
  _dimensions(width, height, depth),
  _sourcePosition(width/2, height/2, depth/2),
  _obstaclePosition(width/2, height/2, depth/2),
  _temperaturePosition(width/2, height/2, depth/2),
  _temperatureValue(30),
  _sourceDensity(0.5),
  _smokeBuoyancy(1.0f),
  _smokeWeight(0.0125f),
  #if FULLSCREEN == 1
    _cubeFront(FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT, 3),
    _cubeBack(FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT, 3)
  #else
    _cubeFront(WINDOW_WIDTH, WINDOW_HEIGHT, 3),
    _cubeBack(WINDOW_WIDTH, WINDOW_HEIGHT, 3)
  #endif
{ 
  _console = Console::getInstance();
  _console->addItem("temp", [&](std::string args)
  {
      std::istringstream is(args);
      float tmp;
      is >> tmp;
      _temperatureValue = tmp;
  }, "float", "Set the temperature.");

  _console->addItem("density", [&](std::string args)
  {
      std::istringstream is(args);
      float tmp;
      is >> tmp;
      _sourceDensity = tmp;
  }, "float", "Set the density.");

  _console->addItem("step", [&](std::string args)
  {
      std::istringstream is(args);
      float tmp;
      is >> tmp;
      _timeStep = tmp;
  }, "float", "Set the time step.");

  _console->addItem("weight", [&](std::string args)
  {
      std::istringstream is(args);
      float tmp;
      is >> tmp;
      _smokeWeight = tmp;
  }, "float", "Set the smoke weight.");

  _console->addItem("buoyancy", [&](std::string args)
  {
      std::istringstream is(args);
      float tmp;
      is >> tmp;
      _smokeBuoyancy = tmp;
  }, "float", "Set the smoke buoyancy.");

  _shaderLoader.loadPrograms("shaders/simulation/calculationPrograms.prog");
  _shaderLoader.loadPrograms("shaders/visualization/viz.prog");

  // Create vbo for fullscreen quad
  GLfloat vertecies[] =
    {
      -1, -1,
       1, -1,
      -1,  1,
       1,  1,
    };
  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertecies), vertecies, GL_STATIC_DRAW);

  // Create vbo for determining volume intersection
  GLfloat volumeVertecies[] = 
  {
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, 
    1.0f, 1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
  };
  glGenBuffers(1, &_volumeVbo);
  glBindBuffer(GL_ARRAY_BUFFER, _volumeVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(volumeVertecies), volumeVertecies, GL_STATIC_DRAW);

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glViewport(0, 0, _dimensions.x, _dimensions.y);

  // initializeObstaclesWithImage(&_obstacles, "hej.png");
  initializeObstacles(&_obstacles);

  _ambientTemperature = 0.0f;
  setTextureValue(_temperature.getBuffer1(), _ambientTemperature);
  setTextureValue(_temperature.getBuffer2(), _ambientTemperature);
}

Simulation::~Simulation()
{

}

void Simulation::stepSimulation()
{ 
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glViewport(0, 0, _dimensions.x, _dimensions.y);

  computeAdvection(_velocity.getBuffer1(), _velocity.getBuffer1(), _velocity.getBuffer2(), &_obstacles);
  _velocity.swapBuffers();

  computeAdvection(_velocity.getBuffer1(), _temperature.getBuffer1(), _temperature.getBuffer2(), &_obstacles);
  _temperature.swapBuffers();

  computeAdvection(_velocity.getBuffer1(), _density.getBuffer1(), _density.getBuffer2(), &_obstacles);
  _density.swapBuffers();

  computeBuoyancy(_velocity.getBuffer1(), _temperature.getBuffer1(), _density.getBuffer1(), _velocity.getBuffer2());
  _velocity.swapBuffers();

  addSource(_temperature.getBuffer1(), _temperaturePosition, glm::vec4(_temperatureValue), 300);

  addSource(_density.getBuffer1(), _sourcePosition, glm::vec4(_sourceDensity,1.0,0.75,0), 5);
  addSource(_density.getBuffer1(), glm::vec3(_dimensions.x/2+80, _dimensions.y/2, _dimensions.z/2), glm::vec4(4.0f/5.0f,3.0f/5.0f,1,0), 5);
  addSource(_density.getBuffer1(), glm::vec3(_dimensions.x/2-80, _dimensions.y/2, _dimensions.z/2), glm::vec4(9.0f/15.0f,15.0f/15.0f,12.0f/15.0f,0), 5);

  addSource(&_obstacles, glm::vec3(_dimensions.x/2-75, _dimensions.y/2 + 40, _dimensions.z/2), glm::vec4(1,0,0,0), 10);
  addSource(&_obstacles, glm::vec3(_dimensions.x/2+75, _dimensions.y/2 + 40, _dimensions.z/2), glm::vec4(1,0,0,0), 10);
  addSource(&_obstacles, glm::vec3(_dimensions.x/2-35, _dimensions.y/2-40, _dimensions.z/2), glm::vec4(1,0,0,0), 10);
  addSource(&_obstacles, glm::vec3(_dimensions.x/2+35, _dimensions.y/2-40, _dimensions.z/2), glm::vec4(1,0,0,0), 10);

  // addSource(&_obstacles, glm::vec3(_dimensions.x/2, _dimensions.y/2 + 40, _dimensions.z/2), glm::vec4(1,0,0,0), 10);

  computeDivergence(_velocity.getBuffer1(), &_divergence, &_obstacles);

  setTextureValue(_pressure.getBuffer1(), 0);

  for (int i = 0; i < 20; ++i)
  {
    computeJacobi(_pressure.getBuffer1(), &_divergence, _pressure.getBuffer2(), &_obstacles);
    _pressure.swapBuffers();
  }

  subtractGradient(_velocity.getBuffer1(), _pressure.getBuffer1(), _velocity.getBuffer2(), &_obstacles);
  _velocity.swapBuffers();

  renderLayer(_density.getBuffer1(), _dimensions.z / 2);

  glDisableVertexAttribArray(0);

  // renderVolume(_velocity.getBuffer1());

}

void Simulation::renderLayer(Volume* source, float renderLayer)
{
  #if FULLSCREEN == 1
    glViewport(0, 0, FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT);
  #else
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  #endif

  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear( GL_COLOR_BUFFER_BIT ); 

  glUseProgram(_shaderLoader.accessProgram("viz2D"));

  setUniform("tex", 0); 
  setUniform("layer", renderLayer);
  setUniform("dimensions", _dimensions);

  #if FULLSCREEN == 1
    setUniform("windowSize", glm::vec2(FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT));
  #else
    setUniform("windowSize", glm::vec2(WINDOW_WIDTH, WINDOW_HEIGHT));
  #endif

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_3D, source->getTexture());

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Simulation::renderVolume(Volume* volume)
{ 
  #if FULLSCREEN == 1
    glViewport(0, 0, FULLSCREEN_WIDTH, FULLSCREEN_HEIGHT);
  #else
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  #endif

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _volumeVbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear( GL_COLOR_BUFFER_BIT ); 
  glUseProgram(_shaderLoader.accessProgram("renderCube"));

  glEnable(GL_DEPTH_TEST);

  // Render front
  glCullFace(GL_BACK);
  glBindFramebuffer(GL_FRAMEBUFFER, _cubeFront.getFbo());
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 36);

  // Render back
  glCullFace(GL_FRONT);
  glBindFramebuffer(GL_FRAMEBUFFER, _cubeBack.getFbo());
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 36);

  glDisableVertexAttribArray(0);
  resetGlState();
}

void Simulation::computeAdvection(Volume* velocity, Volume* source, Volume* destination, Volume* obstacles)
{
  glUseProgram(_shaderLoader.accessProgram("advect"));
  setUniform("sourceTexture", 0); // Bind source texture to unit 0
  setUniform("velocityTexture", 1); // Bind velocity texture to unit 1
  setUniform("obstacleTexture", 2); // Bind obstacle texture to unit 2
  setUniform("timeStep", _timeStep);
  setUniform("dxyz", _gridScale);

  glBindFramebuffer(GL_FRAMEBUFFER, destination->getFbo());
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_3D, source->getTexture());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_3D, velocity->getTexture());
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_3D, obstacles->getTexture());

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _dimensions.z);
  resetGlState();
}

void Simulation::computeJacobi(Volume* pressure, Volume* divergence, Volume* destination, Volume* obstacles)
{
  glUseProgram(_shaderLoader.accessProgram("jacobi"));

  setUniform("pressureTexture", 0); // Bind pressure texture to unit 0
  setUniform("bTexture", 1); // Bind divergence texture to unit 1
  setUniform("obstacleTexture", 2); // Bind obstacle texture to unit 2
  setUniform("alpha", - _dimensions.z * _dimensions.z);
  setUniform("beta", 1/_dimensions.z);

  glBindFramebuffer(GL_FRAMEBUFFER, destination->getFbo());
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_3D, pressure->getTexture());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_3D, divergence->getTexture());
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_3D, obstacles->getTexture());

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _dimensions.z);
  resetGlState();
}

void Simulation::computeDivergence(Volume* velocity, Volume* destination, Volume* obstacles)
{
  glUseProgram(_shaderLoader.accessProgram("divergence"));

  setUniform("velocityTexture", 0); // Bind velocity texture to unit 0
  setUniform("obstacleTexture", 1); // Bind obstacle texture to unit 1
  setUniform("cellSize", _dimensions.z);

  glBindFramebuffer(GL_FRAMEBUFFER, destination->getFbo());
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_3D, velocity->getTexture());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_3D, obstacles->getTexture());

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _dimensions.z);
  resetGlState();
}

void Simulation::computeBuoyancy(Volume* velocity, Volume* temperature, Volume* density, Volume* destination)
{
  glUseProgram(_shaderLoader.accessProgram("buoyancy"));

  setUniform("velocityTexture",0); // velocity Texture
  setUniform("temperatureTexture",1); // temprature texture
  setUniform("densityTexture",2); // density texture

  setUniform("ambientTemperature", _ambientTemperature); // ambientTemperature
  setUniform("timeStep", _timeStep); // timeStep
  setUniform("alpha", _smokeWeight); // alpha
  setUniform("beta", _smokeBuoyancy); // beta

  glBindFramebuffer(GL_FRAMEBUFFER, destination->getFbo());
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_3D, velocity->getTexture());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_3D, temperature->getTexture());
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_3D, density->getTexture());

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _dimensions.z);
  resetGlState();
}

void Simulation::subtractGradient(Volume* velocity, Volume* pressure, Volume* destination, Volume* obstacles)
{
  glUseProgram(_shaderLoader.accessProgram("gradient"));

  setUniform("velocityTexture", 0); // Bind velocity texture to unit 0 
  setUniform("pressureTexture", 1); // Bind pressure texture to unit 1
  setUniform("obstacleTexture", 2); // Bind obstacle texture to unit 2
  setUniform("gridScale", _gridScale.x);

  glBindFramebuffer(GL_FRAMEBUFFER, destination->getFbo());
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_3D, velocity->getTexture());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_3D, pressure->getTexture());
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_3D, obstacles->getTexture());

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _dimensions.z);
  resetGlState();
}

void Simulation::addSource(Volume* destination, glm::vec3 position, glm::vec4 value, float radius)
{
  glUseProgram(_shaderLoader.accessProgram("addSource"));

  setUniform("position", position);
  setUniform("radius", radius);
  setUniform("sourceValue", value);

  glBindFramebuffer(GL_FRAMEBUFFER, destination->getFbo());
  // glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glEnable(GL_BLEND);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _dimensions.z);
  resetGlState();
}

void Simulation::initializeObstacles(Volume* obstacles)
{
  

  glUseProgram(_shaderLoader.accessProgram("initializeObstacles"));

  setUniform("dimensions", _dimensions);

  glBindFramebuffer(GL_FRAMEBUFFER, obstacles->getFbo());
  // glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _dimensions.z);
  resetGlState();
}

void Simulation::initializeObstaclesWithImage(Volume* obstacles, std::string fileName)
{
  std::vector<unsigned char> image;
  unsigned width, height;
  unsigned error = lodepng::decode(image, width, height, fileName.c_str());

  if(error != 0)
  {
    std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
  }else 
    std::cout << "Sucessfully opened obstacle file: " << fileName << std::endl;

  // for (int i = 0; i < image.size(); ++i)
  // {
  //   std::cout << "pix: " << image.at(i) << std::endl;
  // }

  GLuint imageTexture;
  glGenTextures(1, &imageTexture);
  glBindTexture(GL_TEXTURE_2D, imageTexture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

  glUseProgram(_shaderLoader.accessProgram("initializeObstaclesWithImage"));

  setUniform("image", 0);

  glBindFramebuffer(GL_FRAMEBUFFER, obstacles->getFbo());
  // glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, imageTexture);
  glActiveTexture(GL_TEXTURE0);

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _dimensions.z);
  resetGlState();
}


void Simulation::initializeVelocity(Volume* velocity)
{
  glUseProgram(_shaderLoader.accessProgram("initializeVelocity"));
  glBindFramebuffer(GL_FRAMEBUFFER, velocity->getFbo());

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _dimensions.z);
  resetGlState();
}

void Simulation::setUniform(GLuint location, float value)
{
  glUniform1f(location, value);
}

void Simulation::setUniform(GLuint location, int value)
{
  glUniform1i(location, value);
}

void Simulation::setUniform(GLuint location, glm::vec2 value)
{
  glUniform2f(location, value.x, value.y);
}

void Simulation::setUniform(GLuint location, glm::vec3 value)
{
  glUniform3f(location, value.x, value.y, value.z);
}

void Simulation::setUniform(GLuint location, glm::vec4 value)
{
  glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Simulation::setUniform(std::string name, float value)
{
  GLuint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
  GLint location = glGetUniformLocation(program, name.c_str());
  glUniform1f(location, value);
}

void Simulation::setUniform(std::string name, int value)
{
  GLuint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
  GLint location = glGetUniformLocation(program, name.c_str());
  glUniform1i(location, value);
}

void Simulation::setUniform(std::string name, glm::vec2 value)
{
  GLuint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
  GLint location = glGetUniformLocation(program, name.c_str());
  glUniform2f(location, value.x, value.y);
}

void Simulation::setUniform(std::string name, glm::vec3 value)
{
  GLuint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
  GLint location = glGetUniformLocation(program, name.c_str());
  glUniform3f(location, value.x, value.y, value.z);
}

void Simulation::setUniform(std::string name, glm::vec4 value)
{
  GLuint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
  GLint location = glGetUniformLocation(program, name.c_str());
  glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Simulation::resetGlState()
{
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_3D, 0);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_3D, 0);
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_3D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
}

void Simulation::setTextureValue(Volume* texture, float value)
{
  glBindFramebuffer(GL_FRAMEBUFFER, texture->getFbo());
  glClearColor(value, value, value, value);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Simulation::setTextureValue(Volume* texture, float value1, float value2, float value3, float value4)
{
  glBindFramebuffer(GL_FRAMEBUFFER, texture->getFbo());
  glClearColor(value1, value2, value3, value4);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Simulation::addToSourcePosition(glm::vec3 pos)
{
  _sourcePosition += pos;
}

void Simulation::addToObstaclePosition(glm::vec3 pos)
{
  _obstaclePosition += pos;
}