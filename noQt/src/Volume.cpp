#include "Volume.h"

Volume::Volume(int width, int height, int depth, int nrOfComponents)
{
  // Create frambuffer object.
  glGenFramebuffers(1, &_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

  // Create texture and set the correct parameters.
  glGenTextures(1, &_texture);
  glBindTexture(GL_TEXTURE_3D, _texture);
  // Clamps texture coordinates to [0, 1] when outside this range.
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  // Use linear filtering when magnifying or minimizing texture.
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Set number of components per voxel. Always using 16bit floating point.
  switch(nrOfComponents)
  {
    case 1:
      glTexImage3D(GL_TEXTURE_3D, 0, GL_R16F, width, height, depth, 0, GL_RED, GL_HALF_FLOAT, 0);
    break;

    case 2:
      glTexImage3D(GL_TEXTURE_3D, 0, GL_RG16F, width, height, depth, 0, GL_RG, GL_HALF_FLOAT, 0);
    break;

    case 3:
      glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB16F, width, height, depth, 0, GL_RGB, GL_HALF_FLOAT, 0);
    break;

    case 4:
      glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA16F, width, height, depth, 0, GL_RGBA, GL_HALF_FLOAT, 0);
    break;
  }

  // Set for ompimization reasons.
  GLuint renderBuffer;
  glGenRenderbuffers(1, &renderBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
  // Attach render buffer to frambuffer.
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, _texture, 0);

  // Reset state.
  glClearColor(0, 0, 0, 0);
  glClear(GL_COLOR_BUFFER_BIT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  _width = width;
  _height = height;
  _depth = depth;

}

Volume::~Volume()
{

}

GLuint Volume::getFbo()
{
 return _fbo;
}

GLuint Volume::getTexture()
{
  return _texture;
}
