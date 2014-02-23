#ifndef VOLUME_H
#define VOLUME_H

#include <GL/glew.h>

class Volume
{
  public:
    Volume(int width, int height, int depth, int nrOfComponents);
    ~Volume();

    GLuint getFbo();
    GLuint getTexture();
    void clear(float value);
  
  private:
    GLuint _fbo;
    GLuint _texture;

    int _width;
    int _height;
    int _depth;
};

#endif