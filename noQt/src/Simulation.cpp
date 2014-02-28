#include "Simulation.h"

Simulation::Simulation(int width, int height, int depth, float timeStep)
: _density(width, height, depth, 3),
  _velocity(width, height, depth, 3),
  _pressure(width, height, depth, 2),
  _divergence(width, height, depth, 3),
  _obstacles(width, height, depth, 3),
  _timeStep(timeStep),
  _gridScale(1/width, 1/height, 1/depth),
  _dimensions(width, height, depth)
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
  // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  // glClear( GL_COLOR_BUFFER_BIT );

 
  initializeObstacles(&_obstacles);
  addSource(_density.getBuffer2(), glm::vec3(50,50,50), glm::vec4(1, 0, 0, 1), 50);


  render(_density.getBuffer2(), 50);




  // resetGlState();

  // addSource(_density.getBuffer1(), glm::vec3(1,0,0), glm::vec4(0,1,0,1), 0.1);
  // initializeObstacles(&_obstacles);

  // // Advect velocity.
  // computeAdvection(_velocity.getBuffer1(), _velocity.getBuffer1(),  _velocity.getBuffer2(), &_obstacles);
  // _velocity.swapBuffers();

  // // Advect density.
  // computeAdvection(_velocity.getBuffer1(), _density.getBuffer1(), _density.getBuffer2(), &_obstacles);
  // _density.swapBuffers();

  // Add source.
  // addSource(_density.getBuffer2(), glm::vec3(50,50,50), glm::vec4(0.5), 50);

  // // Compute divergence. Stores result in temporary _devergence volume. No need for swap.
  // computeDivergence(_velocity.getBuffer1(), &_divergence, &_obstacles);

  // // Reset the pressure texture and loop Jacobi iterations
  // _pressure.getBuffer1()->clear(0);
  // for (int i = 0; i < 40; ++i)
  // {
  //   computeJacobi(_pressure.getBuffer1(), &_divergence, _pressure.getBuffer2(), &_obstacles);
  //   _pressure.swapBuffers();
  // }
  
  // // Subtract gradient
  // subtractGradient(_velocity.getBuffer1(), _pressure.getBuffer1(), _velocity.getBuffer2(), &_obstacles);
  // _velocity.swapBuffers();

  // render(_density.getBuffer1());

  glDisableVertexAttribArray(0);
}

void Simulation::render(Volume* source, float renderLayer)
{
  // resetGlState();
  glViewport(0, 0, 1024, 768);
  // glViewport(0, 0, 100, 100);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear( GL_COLOR_BUFFER_BIT ); 

  glUseProgram(_shaderLoader.accessProgram("viz2D"));

  // setUniform(0, 0); 
  GLuint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
  GLint location = glGetUniformLocation(program, "tex");
  glUniform1i(location, 0);

  setUniform(1, renderLayer);
  // GLint locations = glGetUniformLocation(program, "layer");
  // glUniform1i(locations, renderLayer);

  setUniform(2, _dimensions);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_3D, source->getTexture());

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
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

void Simulation::resetGlState()
{
    glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_3D, 0);
    glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_3D, 0);
    glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_3D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_BLEND);
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
  resetGlState();
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
  resetGlState();
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
  resetGlState();
}

void Simulation::subtractGradient(Volume* velocity, Volume* pressure, Volume* destination, Volume* obstacles)
{
  glUseProgram(_shaderLoader.accessProgram("gradient"));

  setUniform(0, 0); // Bind velocity texture to unit 0 
  setUniform(1, 1); // Bind pressure texture to unit 1
  setUniform(2, 2); // Bind obstacle texture to unit 2
  setUniform(3, _gridScale.x);

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

  setUniform(0, position);
  setUniform(1, radius);
  setUniform(2, value);

  glBindFramebuffer(GL_FRAMEBUFFER, destination->getFbo());
  // glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // glEnable(GL_BLEND);
  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _dimensions.z);
  resetGlState();


  // glUseProgram(_shaderLoader.accessProgram("addSource"));

  // setUniform(0, glm::vec3(50,50,0));
  // setUniform(1, 20);
  // setUniform(2, glm::vec4(1,0,0,0));

  // glBindFramebuffer(GL_FRAMEBUFFER, 0);
  // // glEnable(GL_BLEND);
  // glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _dimensions.z);


}

void Simulation::initializeObstacles(Volume* obstacles)
{
  glUseProgram(_shaderLoader.accessProgram("initializeObstacles"));

  setUniform(0, _dimensions);

  glBindFramebuffer(GL_FRAMEBUFFER, obstacles->getFbo());
  // glBindFramebuffer(GL_FRAMEBUFFER, 0);

  glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, _dimensions.z);
  resetGlState();
}