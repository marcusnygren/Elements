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
    Simulation(int width, int height, int depth, float timeStep);
    ~Simulation();
    
    void stepSimulation();
    void renderLayer(Volume* source, float renderLayer);

    void addToSourcePosition(glm::vec3 pos);

  private:
    void computeAdvection(Volume* velocity, Volume* source, Volume* destination, Volume* obstacles);
    void computeJacobi(Volume* pressure, Volume* divergence, Volume* destination, Volume* obstacles);
    void computeDivergence(Volume* velocity, Volume* destination, Volume* obstacles);
    void subtractGradient(Volume* velocity, Volume* pressure, Volume* destination, Volume* obstacles);
    void computeBuoyancy(Volume* velocity, Volume* temperature, Volume* density, Volume* destination);
    void addSource(Volume* destination, glm::vec3 position, glm::vec4 value, float radius);
    void initializeObstacles(Volume* obstacles);
    void initializeVelocity(Volume* velocity);

    void resetGlState();
    void setTextureValue(Volume* texture, float value);
    void setTextureValue(Volume* texture, float value1, float value2, float value3, float value4);

    void setUniform(GLuint location, float value);
    void setUniform(GLuint location, int value);
    void setUniform(GLuint location, glm::vec3 value);
    void setUniform(GLuint location, glm::vec4 value);


    GLuint _vbo;

    Loader _shaderLoader;

    Container _density;
    Container _velocity;
    Container _pressure;
    Container _temperature;

    Volume _obstacles;
    Volume _divergence;

    float _timeStep;
    glm::vec3 _gridScale;
    glm::vec3 _dimensions;


    float _ambientTemperature;
    float _smokeBuoyancy = 1.0f;
    float _smokeWeight = 0.0125f;
    glm::vec3 _sourcePosition;
};

#endif