#version 330

out vec3 color;

uniform sampler3D tex;

uniform float layer;
uniform vec3 dimensions;

void main()
{
  // if (layer < dimensions.z)
  // {
  //   if (gl_FragCoord.x < dimensions.x && gl_FragCoord.y < dimensions.y)
  //   {
  //     color = texture(tex, vec3(gl_FragCoord.x / 100,gl_FragCoord.y / 100, layer/100)).xyz;
  //   }else
  //     // color = vec3(0.4,0.4,0.4);
  //     color = vec3(gl_FragCoord.x / 1024,gl_FragCoord.y / 768, layer /100);
  // }else
  //  color = vec3(1,0,0);

  color = texture(tex, vec3(gl_FragCoord.x / 300,gl_FragCoord.y / 300, layer/100)).xyz;

}