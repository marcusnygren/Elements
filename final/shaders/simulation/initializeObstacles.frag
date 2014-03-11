#version 330

out vec3 value;

uniform vec3 dimensions;

in float layer;

void main()
{ 
  int size = 1;
  vec3 position = vec3(gl_FragCoord.xy, layer);
  if (position.x < size ||
      position.x > dimensions.x - size ||
      position.y < size ||
      position.y > dimensions.y - size ||
      position.z < size ||
      position.z > dimensions.z - size )
  {
    value = vec3(1,0,0);
  }
  else
  {
    value = vec3(0,0,0);
  }

}