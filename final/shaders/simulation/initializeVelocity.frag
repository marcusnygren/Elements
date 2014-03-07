#version 330

out vec4 value;

in float layer;

void main()
{ 
  vec3 position = vec3(gl_FragCoord.xy, layer);
  if (position.x <= 10 ||
      position.x >= 100 - 10 ||
      position.y <= 10 ||
      position.y >= 100 - 10 ||
      position.z <= 10 ||
      position.z >= 100 - 10 )
  {
    value = vec4(0,0,0,0);
  }else
  {
    float scale = 10;
    value.x = (fract(sin(dot(gl_FragCoord.xy ,vec2(12.9898,78.233))) * 43758.5453) + 0.1) * scale;
    value.y = (fract(sin(dot(gl_FragCoord.xy ,vec2(12.9898,78.233))) * 43758.5453) + 0.1) * scale;
    value.z = (fract(sin(dot(gl_FragCoord.xy ,vec2(12.9898,78.233))) * 43758.5453) + 0.1) * scale;
    value.w = (fract(sin(dot(gl_FragCoord.xy ,vec2(12.9898,78.233))) * 43758.5453) + 0.1) * scale;
  }

  
}