#version 330
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_shading_language_420pack : enable

out vec4 color;

layout (location = 0) uniform vec3 position;
layout (location = 1) uniform float radius;
layout (location = 2) uniform vec4 sourceValue;

flat in int layer;

void main()
{
  vec3 currentPosition = vec3(gl_FragCoord.xy, layer);
  float distance = distance(position, currentPosition);

  if (distance < radius)
  {
    color = sourceValue;
  }
  else 
    color = vec4(0);
}