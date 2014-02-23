#include "Simulation.h"

Simulation::Simulation(int width, int height, int depth, float timeStep)
: _density(width, height, depth, 1),
  _velocity(width, height, depth, 3),
  _pressure(width, height, depth, 1),
  _divergence(width, height, depth, 3),
  _obstacles(width, height, depth, 3),
  _timeStep(timeStep),
  _gridScale(1/width, 1/height, 1/depth),
  _dimensions(width, height, depth)
{

  _shaderLoader.loadPrograms("shaders/simulation/calculationPrograms.prog");

}

Simulation::~Simulation()
{

}

void Simulation::setUniform(GLuint location, float value)
{
  // GLuint program;
  // glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
  glUniform1f(location, value);
}

void Simulation::setUniform(GLuint location, glm::vec3 value)
{
  // GLuint program;
  // glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
  glUniform3f(location, value.x, value.y, value.z);
}

void Simulation::setUniform(GLuint location, glm::vec4 value)
{
  // GLuint program;
  // glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
  glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Simulation::computeAdvection(Volume* velocity, Volume* source, Volume* destination, Volume* obstacles)
{
  glUseProgram(_shaderLoader.accessProgram("advect"));
  setUniform(0, 0); // Bind source texture to unit 0
  setUniform(1, 1); // Bind velocity texture to unit 1
  setUniform(2, 2); // Bind obstacle texture to unit 2
  setUniform(3, _timeStep);
  setUniform(4, _gridScale);

  glBindFramebuffer(GL_FRAMEBUFFER, destination->getFbo());
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_3D, source->getTexture());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_3D, velocity->getTexture());
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_3D, obstacles->getTexture());

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _dimensions.z);
}

void Simulation::computeJacobi(Volume* pressure, Volume* divergence, Volume* destination, Volume* obstacles)
{
  glUseProgram(_shaderLoader.accessProgram("jacobi"));

  setUniform(0, 0); // Bind pressure texture to unit 0
  setUniform(1, 1); // Bind divergence texture to unit 1
  setUniform(2, 2); // Bind obstacle texture to unit 2
  setUniform(3, - _dimensions.z * _dimensions.z);
  setUniform(4, 6);

  glBindFramebuffer(GL_FRAMEBUFFER, destination->getFbo());
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_3D, pressure->getTexture());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_3D, divergence->getTexture());
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_3D, obstacles->getTexture());

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _dimensions.z);
}

void Simulation::computeDivergence(Volume* velocity, Volume* destination, Volume* obstacles)
{
  glUseProgram(_shaderLoader.accessProgram("divergence"));

  setUniform(0, 0); // Bind velocity texture to unit 0
  setUniform(1, 1); // Bind obstacle texture to unit 1
  setUniform(2, _dimensions.z); // borde vara den här eller ? kanske gridScale???

  glBindFramebuffer(GL_FRAMEBUFFER, destination->getFbo());
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_3D, velocity->getTexture());
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_3D, obstacles->getTexture());

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _dimensions.z);
}

void Simulation::subtractGradient(Volume* velocity, Volume* pressure, Volume* destination, Volume* obstacles)
{
  
}

void Simulation::addSource(Volume* destination, glm::vec3 position, glm::vec4 value, float radius)
{
  glUseProgram(_shaderLoader.accessProgram("addSource"));

  setUniform(0, position);
  setUniform(1, radius);
  setUniform(2, value);

  glBindFramebuffer(GL_FRAMEBUFFER, destination->getFbo());
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _dimensions.z);
}