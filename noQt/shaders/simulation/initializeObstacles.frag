#version 330

out vec4 value;

layout (location = 0) uniform vec3 dimensions;

in int layer;

void main()
{
  vec3 position = vec3(gl_FragCoord.xy, layer);
  if (position.x < 10 ||
      position.x > dimensions.x - 10 ||
      position.y < 10 ||
      position.y > dimensions.y - 10 ||
      position.z < 10 ||
      position.z > dimensions.z - 10)
  {
    value = vec4(1);
  }
  else
  {
    value = vec4(0);
  }

}