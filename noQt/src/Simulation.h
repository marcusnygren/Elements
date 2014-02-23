#ifndef SIMULATION_H
#define SIMULATION_H 

#include <GL/glew.h>
#include "glm/glm.hpp"

#include "Loader.h"
#include "Container.h"
#include "Volume.h"

class Simulation
{
  public:
    Simulation(int width, int height, int depth, float timeStep, float startTemperature);
    ~Simulation();
    
    void stepSimulation();
    void render();

  private:
    void computeAdvection(Volume* velocity, Volume* source, Volume* destination, Volume* obstacles);
    void computeJacobi(Volume* pressure, Volume* divergence, Volume* destination, Volume* obstacles);
    void computeDivergence(Volume* velocity, Volume* destination, Volume* obstacles);
    void subtractGradient(Volume* velocity, Volume* pressure, Volume* destination, Volume* obstacles);
    void addSource(Volume* destination, glm::vec3 position, glm::vec4 value, float radius);
    void addBuoncy(Volume* velocity, Volume* temperature, Volume* density, Volume* destination);

    void setUniform(GLuint location, float value);
    void setUniform(GLuint location, glm::vec3 value);
    void setUniform(GLuint location, glm::vec4 value);


    Loader _shaderLoader;

    Container _velocity;
    Container _pressure;
    Container _temperature;
    Container _density;

    Volume _obstacles;
    Volume _divergence;

    float _timeStep;
    float _startTemperature;
    float _startDensity;

    glm::vec3 _gridScale;
    glm::vec3 _dimensions;
};

#endif