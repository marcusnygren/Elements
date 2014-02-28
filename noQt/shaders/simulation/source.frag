#version 330
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_shading_language_420pack : enable

out vec3 color;

layout (location = 0) uniform vec3 position;
layout (location = 1) uniform float radius;
layout (location = 2) uniform vec4 sourceValue;

flat in float layer;

void main()
{
  vec3 currentPosition = vec3(gl_FragCoord.xy, layer);
  float d = distance(position, currentPosition);

  if (d < radius)
  {
    // float a = (radius - d) * 0.5;
    // a = min(a, 1.0);
    // color = vec4(sourceValue.xyz, a);
    color = sourceValue.xyz;
    // color = vec3(0, 1, 0);
  }
  else 
    color = vec4(0).xyz;
  // if (gl_FragCoord.z == 0.5)
  //   color = vec3(0,1,0);
  // else
  //   color = vec3(layer/10,0,0);

  // color = vec3(1,0,0);
}