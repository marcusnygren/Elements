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
    // color = sourceValue.xyz;
    // color = vec3(0, 1, 0);
  }
  else 
    color = vec4(0);
  // if (gl_FragCoord.z == 0.5)
  //   color = vec3(0,1,0);
  // else
  //   color = vec3(layer/10,0,0);

  // color = vec3(1,0,0);
}