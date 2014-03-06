#ifndef SURFACE_H
#define SURFACE_H

#include <iostream>

#include <GL/glew.h>

class Surface
{
  public:
    Surface(int width, int height, int nrOfComponents);
    ~Surface();

    GLuint getFbo();
    GLuint getTexture();
    void clear(float value);
  private:
    GLuint _fbo;
    GLuint _texture;

    int _width;
    int _height;
  
};

#endif