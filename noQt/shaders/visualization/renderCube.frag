#version 330

out vec3 value;

void main()
{
  value =  gl_FragCoord.xyz;
}