#version 330
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_shading_language_420pack : enable

out vec3 color;

layout (binding = 0) uniform sampler3D tex;

layout (location = 1) uniform int layer;
layout (location = 2) uniform vec3 dimensions;

void main()
{
  if (layer < dimensions.z)
  {
    if (gl_FragCoord.x < dimensions.x && gl_FragCoord.y < dimensions.y)
    {
      color = texture(tex, vec3(gl_FragCoord.xy, layer)).xyz;
      // if ()
      //   color = vec3(0,1,0);
      // else
      //   color = vec3(0,0,1);
    }else
      color = vec3(0.4,0.4,0.4);
  }else
   color = vec3(1,0,0);

}