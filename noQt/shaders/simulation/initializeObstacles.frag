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

  // if (layer > 0 && layer < 70)
  //   value = vec4(0,1,0,1);
  // else
  //   value =  vec4(1,0,0,1);

 // value =  vec4(1,0,0,1);

  // value = vec4(gl_FragCoord.x / 100,gl_FragCoord.y / 100,0,1);

}