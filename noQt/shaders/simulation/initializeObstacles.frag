#version 330
#extension GL_ARB_explicit_attrib_location : enable
#extension GL_ARB_explicit_uniform_location : enable
#extension GL_ARB_shading_language_420pack : enable

out vec4 value;

layout(location = 0) uniform vec3 dimensions;

flat in float layer;

void main()
{
  vec3 position = vec3(gl_FragCoord.xy, layer);
  if (position.x < 10 ||
      position.x > dimensions.x - 10 ||
      position.y < 10 ||
      position.y > dimensions.y - 10 ||
      position.z < 10 ||
      position.z > dimensions.z - 10 )
  {
    value = vec4(1,0,0,1);
  }
  else
  {
    value = vec4(0,0,0,1);
  }

  // if (layer > 0 && layer < 70)
  //   value = vec4(0,1,0,1);
  // else
  //   value =  vec4(1,0,0,1);

 // value =  vec4(1,0,0,1);

  // value = vec4(gl_FragCoord.x / 100,gl_FragCoord.y / 100,0,1);

}