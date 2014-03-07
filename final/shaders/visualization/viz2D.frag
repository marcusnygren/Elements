#version 330

out vec3 color;

uniform sampler3D tex;

uniform float layer;
uniform vec3 dimensions;
uniform vec2 windowSize;

void main()
{
  color = texture(tex, vec3(gl_FragCoord.x / windowSize.x ,gl_FragCoord.y / windowSize.y, layer/ dimensions.z)).xyz;
}