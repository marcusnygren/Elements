#version 330

out vec3 value;

uniform sampler2D image;

in float layer;

void main()
{

  if (texture(image, vec2(gl_FragCoord.x/300, gl_FragCoord.y/300)).x < 0.99)
    value = vec3(1,0,0);
  else
    value = vec3(0,0,0);

  // value.x = texture(image, vec2(gl_FragCoord.x/300, gl_FragCoord.y/300)).x;
}