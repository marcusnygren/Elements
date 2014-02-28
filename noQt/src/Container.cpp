#include "Container.h"

Container::Container(int width, int height, int depth, int nrOfComponents)
{
  _buffer1 = new Volume(width, height, depth, nrOfComponents);
  _buffer2 = new Volume(width, height, depth, nrOfComponents);
}

Container::~Container()
{
  delete _buffer1;
  delete _buffer2;
}

void Container::swapBuffers()
{
  _tempBuffer = _buffer1;
  _buffer1 = _buffer2;
  _buffer2 = _tempBuffer;
}

Volume* Container::getBuffer1()
{
  return _buffer1;
}

Volume* Container::getBuffer2()
{
  return _buffer2;
}