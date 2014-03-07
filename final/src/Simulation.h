#ifndef SIMULATION_H
#define SIMULATION_H 

#include <string>

#include <GL/glew.h>
#include "glm/glm.hpp"

#include "Loader.h"
#include "Container.h"
#include "Volume.h"
#include "Surface.h"

class Simulation
{
  public:
    Simulation(int width, int height, int depth, float timeStep);
    ~Simulation();
    
    void stepSimulation();
    void renderLayer(Volume* source, float renderLayer);
    void renderVolume(Volume* volume);

    void addToSourcePosition(glm::vec3 pos);
    void addToObstaclePosition(glm::vec3 pos);

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
    void setUniform(GLuint location, glm::vec2 value);
    void setUniform(GLuint location, glm::vec3 value);
    void setUniform(GLuint location, glm::vec4 value);

    void setUniform(std::string name, float value);
    void setUniform(std::string name, int value);
    void setUniform(std::string name, glm::vec2 value);
    void setUniform(std::string name, glm::vec3 value);
    void setUniform(std::string name, glm::vec4 value);


    GLuint _vbo;
    GLuint _volumeVbo;

    Loader _shaderLoader;

    Container _density;
    Container _velocity;
    Container _pressure;
    Container _temperature;

    Volume _obstacles;
    Volume _divergence;

    Surface _cubeFront;
    Surface _cubeBack;

    float _timeStep;
    glm::vec3 _gridScale;
    glm::vec3 _dimensions;


    float _ambientTemperature;
    float _smokeBuoyancy = 1.0f;
    float _smokeWeight = 0.0125f;
    glm::vec3 _sourcePosition;
    glm::vec3 _obstaclePosition;
    glm::vec3 _temperaturePosition;
};

#endif