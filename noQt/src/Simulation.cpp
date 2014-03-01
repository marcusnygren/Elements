#include "Simulation.h"

#include <iostream>
Simulation::Simulation(int width, int height, int depth, float timeStep)
: _density(width, height, depth, 3),
  _velocity(width, height, depth, 3),
  _pressure(width, height, depth, 2),
  _divergence(width, height, depth, 3),
  _obstacles(width, height, depth, 3),
  _temperature(width, height, depth, 2),
  _timeStep(timeStep),
  _gridScale(1.0f/(float)width, 1.0f/ (float)height, 1.0f/(float) depth),
  _dimensions(width, height, depth),
  _sourcePosition(50, 50, 50)
{ 

  _shaderLoader.loadPrograms("shaders/simulation/calculationPrograms.prog");
  _shaderLoader.loadPrograms("shaders/visualization/viz2D.prog");

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

  glDisable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glViewport(0, 0, 100, 100);

  initializeObstacles(&_obstacles);

  _ambientTemperature = 0.0f;
  setTextureValue(_temperature.getBuffer1(), _ambientTemperature);
  setTextureValue(_temperature.getBuffer2(), _ambientTemperature);


  // initializeVelocity(_velocity.getBuffer1());
  // initializeVelocity(_velocity.getBuffer2());
  // setTextureValue(_velocity.getBuffer1(), -0.75,0.5,1,1);
  // setTextureValue(_velocity.getBuffer2(), -0.75,0.5,1,1);

  // setTextureValue(_velocity.getBuffer1(), -0.91,0.1,0.0,1);
  // setTextureValue(_velocity.getBuffer2(), -0.91,0.1,0.0,1);

  // initializeObstacles(_velocity.getBuffer1());

  // initializeObstacles(_velocity.getBuffer2());
}

Simulation::~Simulation()
{

}

void Simulation::stepSimulation()
{ 
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  glViewport(0, 0, 100, 100);


  // initializeVelocity(_velocity.getBuffer1());
  // initializeVelocity(_velocity.getBuffer2());

  computeAdvection(_velocity.getBuffer1(), _velocity.getBuffer1(), _velocity.getBuffer2(), &_obstacles);
  _velocity.swapBuffers();

  computeAdvection(_velocity.getBuffer1(), _temperature.getBuffer1(), _temperature.getBuffer2(), &_obstacles);
  _temperature.swapBuffers();

  computeAdvection(_velocity.getBuffer1(), _density.getBuffer1(), _density.getBuffer2(), &_obstacles);
  _density.swapBuffers();

  computeBuoyancy(_velocity.getBuffer1(), _temperature.getBuffer1(), _density.getBuffer1(), _velocity.getBuffer2());
  _velocity.swapBuffers();

  addSource(_temperature.getBuffer1(), glm::vec3(50, 50, 50), glm::vec4(20), 20);

  addSource(_density.getBuffer1(), _sourcePosition, glm::vec4(1,0.2,1,0), 5);

  computeDivergence(_velocity.getBuffer1(), &_divergence, &_obstacles);

  setTextureValue(_pressure.getBuffer1(), 0);

  for (int i = 0; i < 40; ++i)
  {
    computeJacobi(_pressure.getBuffer1(), &_divergence, _pressure.getBuffer2(), &_obstacles);
    _pressure.swapBuffers();
  }

  subtractGradient(_velocity.getBuffer1(), _pressure.getBuffer1(), _velocity.getBuffer2(), &_obstacles);
  _velocity.swapBuffers();

  renderLayer(_density.getBuffer1(), 50);

  glDisableVertexAttribArray(0);

}

void Simulation::renderLayer(Volume* source, float renderLayer)
{
  // resetGlState();
  glViewport(0, 0, 300, 300);
  // glViewport(0, 0, 100, 100);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear( GL_COLOR_BUFFER_BIT ); 

  glUseProgram(_shaderLoader.accessProgram("viz2D"));

  setUniform("tex", 0); 
  setUniform("layer", renderLayer);
  setUniform("dimensions", _dimensions);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_3D, source->getTexture());

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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
  setUniform("beta", 6);

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
  setUniform("cellSize", _dimensions.z); // borde vara den här eller ? kanske gridScale???

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