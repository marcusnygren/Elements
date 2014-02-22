#ifndef CONTAINER_H
#define CONTAINER_H

#include <GL/glew.h>

#include "Volume.h"

class Container
{
  public:
    Container(int width, int height, int depth, int nrOfComponents);
    ~Container();

    void swapBuffers();

    Volume* getBuffer1();
    Volume* getBuffer2();

  private:
    Volume* _buffer1;
    Volume* _buffer2;
    Volume* _tempBuffer;
  
};

#endif