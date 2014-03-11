#version 330

out vec4 color;

uniform vec3 position;
uniform float radius;
uniform vec4 sourceValue;

in float layer;

void main()
{
  vec3 currentPosition = vec3(gl_FragCoord.xy, layer);
  float d = distance(position, currentPosition);

  if (d < radius)
  {
    float a = (radius - d) * 0.5;
    a = min(a, 1.0);
    color = vec4(sourceValue.xyz, a);
  }
  else 
    color = vec4(0);
}