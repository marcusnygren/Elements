#version 330

out vec4 color;

layout (location = 0) uniform vec3 position;
layout (location = 1) uniform float radius;
layout (location = 2) uniform vec4 sourceValue;

in int layer;

void main()
{
  vec3 currentPosition(gl_FragCoord.xy, layer);
  float distance = distance(position, currentPosition);

  if (distance < radius)
  {
    color = sourceValue;
  }
  else 
    color = vec4(0);
}