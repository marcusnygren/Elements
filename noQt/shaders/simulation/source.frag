#version 330

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
    float a = (radius - distance) * 0.5;
    a = min(a, 1.0);
    color = vec4(sourceValue.xyz, a);
        
    // color = sourceValue;
  }
  else 
    color = vec4(0);
}